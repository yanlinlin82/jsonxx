#include "json.hpp"

void test_constructors()
{
	Json j0;                      // j0 = null
	Json j1(false);               // j1 = false
	Json j2(true);                // j2 = true
	Json j3(0);                   // j3 = 0
	Json j4(123);                 // j4 = 123
	Json j5(-45);                 // j5 = -45
	Json j6(1234567890123LL);     // j6 = 1234567890123
	Json j7(0.1f);                // j7 = 0.100000
	Json j8(3.14159);             // j8 = 3.141590
	Json j9("abc");               // j9 = "abc"
	Json j10(std::string("xyz")); // j10 = "xyz"
}

void test_assignments()
{
	Json j0 = Json();              // j0 = null
	Json j1 = false;               // j1 = false
	Json j2 = true;                // j2 = true
	Json j3 = 0;                   // j3 = 0
	Json j4 = 123;                 // j4 = 123
	Json j5 = -45;                 // j5 = -45
	Json j6 = 1234567890123LL;     // j6 = 1234567890123
	Json j7 = 0.1f;                // j7 = 0.100000
	Json j8 = 3.14159;             // j8 = 3.141590
	Json j9 = "abc";               // j9 = "abc"
	Json j10 = std::string("xyz"); // j10 = "xyz"
}

void test_array_and_object()
{
	Json j0(Json().Append("abc"));      // j0 = ["abc"]

	Json j1(Json().Append("key", 123)); // j1 = {"key":123}

	Json j2("abc");         // j2 = "abc"
	j2.Append("x", 123);    // j2 = {"x":123}
	j2.Append("y", Json()); // j2 = {"x":123,"y":null}

	Json j3("abc");    // j3 = "abc"
	j3.Append(123);    // j3 = [123]
	j3.Append(Json()); // j3 = [123,null]

	Json j4;   // j4 = null
	j4 += 123; // j4 = [123]
	j4 += 45;  // j4 = [123,45]

	Json j5;                                                        // j5 = null
	j5 += std::make_pair("key", 123);                               // j5 = {"key":123}
	j5 += std::make_pair("second", Json().Append("a").Append("b")); // j5 = {"key":123,"second":["a","b"]}

	Json j6;           // j6 = null
	j6["x"] = 123;     // j6 = {"x":123}
	j6["y"] = "hello"; // j6 = {"x":123,"y":"hello"}
	j6["z"][2] = true; // j6 = {"x":123,"y":"hello","z":[null,null,true]}
}

void test_parser()
{
	Json j;
	j.Parse("null");  // j = null
	j.Parse("false"); // j = false
	j.Parse("true");  // j = true
}

int main()
{
	test_constructors();
	test_assignments();
	test_array_and_object();
	test_parser();
	return 0;
}
