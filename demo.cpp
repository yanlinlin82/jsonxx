#include "json.hpp"

using yll::json;

void test_constructors()
{
	// null json
	json j0; // j0 = null

	// boolean json
	json j1(false); // j1 = false
	json j2(true);  // j2 = true

	// number json
	json j3(0);               // j3 = 0
	json j4(123);             // j4 = 123
	json j5(-45);             // j5 = -45
	json j6(1234567890123LL); // j6 = 1234567890123
	json j7(0.1f);            // j7 = 0.1
	json j8(3.14159);         // j8 = 3.14159

	// string json
	json j9("C-style string");          // j9 = "C-style string"
	json j10(std::string("C++ style")); // j10 = "C++ style"

	// array json
	json j21({"a", "b"});         // j21 = ["a","b"]
	json j22({123, 456, 78, 90}); // j22 = [123,456,78,90]
	json j23({"123", "xyz"});     // j23 = ["123","xyz"]
	json j24({1.23, 45.6, .78});  // j24 = [1.23,45.6,0.78]

	// object json
	json j31("key", "value"); // j31 = {"key":"value"}
	json j32("pi", 3.14159);  // j32 = {"pi":3.14159}

	// more complex json
	json j41("array", {1, 2, 3}); // j41 = {"array":[1,2,3]}
}

void test_json_node_values()
{
	json j1(true);  // j1.as_boolean() = 1
	json j2(false); // j2.as_boolean() = 0

	json j3(123);  // j3.as_number() = 123
	json j4(3.14); // j4.as_number<float>() = 3.14

	json j5("hello"); // j5.as_string() = hello
}

void test_assignments()
{
	json j;

	// null json
	j = json(); // j = null

	// boolean json
	j = false; // j = false
	j = true;  // j = true

	// number json
	j = 0;               // j = 0
	j = 123;             // j = 123
	j = -45;             // j = -45
	j = 1234567890123LL; // j = 1234567890123
	j = 0.1f;            // j = 0.1
	j = 3.14159;         // j = 3.14159

	// string json
	j = "C-style string";         // j = "C-style string"
	j = std::string("C++ style"); // j = "C++ style"

	// array json
	j = {"a", "b"};         // j = ["a","b"]
	j = {123, 456, 78, 90}; // j = [123,456,78,90]
	j = {"123", "xyz"};     // j = ["123","xyz"]
	j = {1.23, 45.6, .78};  // j = [1.23,45.6,0.78]

	// object json
	j = json("key", "value"); // j = {"key":"value"}
	j = json("pi", 3.14159);  // j = {"pi":3.14159}

	// more complex json
	j = json("array", {1, 2, 3}); // j = {"array":[1,2,3]}
}

void test_array_and_object()
{
	json j({"abc"});         // j = ["abc"]
	j.append("def");         // j = ["abc","def"]
	j.append({"uvw","xyz"}); // j = ["abc","def","uvw","xyz"]
	j.append(123);           // j = ["abc","def","uvw","xyz",123]
	j.append({3.14,-2.18});  // j = ["abc","def","uvw","xyz",123,3.14,-2.18]

	json j1("key",123);      // j1 = {"key":123}
	j1.append("foo","bar");  // j1 = {"key":123,"foo":"bar"}

	json j2("abc");         // j2 = "abc"
	j2.append("x", 123);    // j2 = {"x":123}
	j2.append("y", json()); // j2 = {"x":123,"y":null}

	json j3("abc");    // j3 = "abc"
	j3.append(123);    // j3 = ["abc",123]
	j3.append(json()); // j3 = ["abc",123]

	json j4;   // j4 = null
	j4 += 123; // j4 = 123
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
	std::cout << std::unitbuf; // set no buffer for stdout
	test_constructors();
	test_json_node_values();
	test_assignments();
	test_array_and_object();
	test_parser();
	return 0;
}
