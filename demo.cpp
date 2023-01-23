#include "json.hpp"

using yll::json;

void test_constructors()
{
	json j0;                      // j0 = null
	json j1(false);               // j1 = false
	json j2(true);                // j2 = true
	json j3(0);                   // j3 = 0
	json j4(123);                 // j4 = 123
	json j5(-45);                 // j5 = -45
	json j6(1234567890123LL);     // j6 = 1234567890123
	json j7(0.1f);                // j7 = 0.100000
	json j8(3.14159);             // j8 = 3.141590
	json j9("abc");               // j9 = "abc"
	json j10(std::string("xyz")); // j10 = "xyz"

	json j11({"a", "b"});         // j11 = ["a","b"]
}

void test_assignments()
{
	json j0 = json();              // j0 = null
	json j1 = false;               // j1 = false
	json j2 = true;                // j2 = true
	json j3 = 0;                   // j3 = 0
	json j4 = 123;                 // j4 = 123
	json j5 = -45;                 // j5 = -45
	json j6 = 1234567890123LL;     // j6 = 1234567890123
	json j7 = 0.1f;                // j7 = 0.100000
	json j8 = 3.14159;             // j8 = 3.141590
	json j9 = "abc";               // j9 = "abc"
	json j10 = std::string("xyz"); // j10 = "xyz"
}

void test_array_and_object()
{
	json j0(json().append("abc"));      // j0 = ["abc"]

	json j1(json().append("key", 123)); // j1 = {"key":123}

	json j2("abc");         // j2 = "abc"
	j2.append("x", 123);    // j2 = {"x":123}
	j2.append("y", json()); // j2 = {"x":123,"y":null}

	json j3("abc");    // j3 = "abc"
	j3.append(123);    // j3 = [123]
	j3.append(json()); // j3 = [123,null]

	json j4;   // j4 = null
	j4 += 123; // j4 = [123]
	j4 += 45;  // j4 = [123,45]

	json j5;                        // j5 = null
	j5.append("key", 123);          // j5 = {"key":123}
	j5.append("second", {"a","b"}); // j5 = {"key":123,"second":["a","b"]}

	json j6;           // j6 = null
	j6["x"] = 123;     // j6 = {"x":123}
	j6["y"] = "hello"; // j6 = {"x":123,"y":"hello"}
	j6["z"][2] = true; // j6 = {"x":123,"y":"hello","z":[null,null,true]}
}

void test_parser()
{
	json j;
	j.parse("null");  // j = null
	j.parse("false"); // j = false
	j.parse("true");  // j = true
}

int main()
{
	test_constructors();
	test_assignments();
	test_array_and_object();
	test_parser();
	return 0;
}
