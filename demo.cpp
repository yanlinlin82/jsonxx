#include "json.hpp"

void test_constructors()
{
	Json j0;
	std::cout << __func__ << ": j0 = " << j0 << std::endl;

	Json j1(false);
	std::cout << __func__ << ": j1 = " << j1 << std::endl;

	Json j2(true);
	std::cout << __func__ << ": j2 = " << j2 << std::endl;

	Json j3(0);
	std::cout << __func__ << ": j3 = " << j3 << std::endl;

	Json j4(123);
	std::cout << __func__ << ": j4 = " << j4 << std::endl;

	Json j5(-45);
	std::cout << __func__ << ": j5 = " << j5 << std::endl;

	Json j6(1234567890123LL);
	std::cout << __func__ << ": j6 = " << j6 << std::endl;

	Json j7(0.1f);
	std::cout << __func__ << ": j7 = " << j7 << std::endl;

	Json j8(3.14159);
	std::cout << __func__ << ": j8 = " << j8 << std::endl;

	Json j9("abc");
	std::cout << __func__ << ": j9 = " << j9 << std::endl;

	Json j10(std::string("xyz"));
	std::cout << __func__ << ": j10 = " << j10 << std::endl;
}

void test_assignments()
{
	Json j0 = Json();
	std::cout << __func__ << ": j0 = " << j0 << std::endl;

	Json j1 = false;
	std::cout << __func__ << ": j1 = " << j1 << std::endl;

	Json j2 = true;
	std::cout << __func__ << ": j2 = " << j2 << std::endl;

	Json j3 = 0;
	std::cout << __func__ << ": j3 = " << j3 << std::endl;

	Json j4 = 123;
	std::cout << __func__ << ": j4 = " << j4 << std::endl;

	Json j5 = -45;
	std::cout << __func__ << ": j5 = " << j5 << std::endl;

	Json j6 = 1234567890123LL;
	std::cout << __func__ << ": j6 = " << j6 << std::endl;

	Json j7 = 0.1f;
	std::cout << __func__ << ": j7 = " << j7 << std::endl;

	Json j8 = 3.14159;
	std::cout << __func__ << ": j8 = " << j8 << std::endl;

	Json j9 = "abc";
	std::cout << __func__ << ": j9 = " << j9 << std::endl;

	Json j10 = std::string("xyz");
	std::cout << __func__ << ": j10 = " << j10 << std::endl;
}

void test_array_and_object()
{
	Json j0(Json().Append("abc"));
	std::cout << __func__ << ": j0 = " << j0 << std::endl;

	Json j1(Json().Append("key", 123));
	std::cout << __func__ << ": j1 = " << j1 << std::endl;

	Json j2("abc");
	j2.Append("x", 123);
	std::cout << __func__ << ": j2 = " << j2 << std::endl;

	j2.Append("y", Json());
	std::cout << __func__ << ": j2 = " << j2 << std::endl;

	std::cout << __func__ << ": j2[\"1\"] = " << j2["1"] << std::endl;
	std::cout << __func__ << ": j2[\"x\"] = " << j2["x"] << std::endl;
	std::cout << __func__ << ": j2[\"y\"] = " << j2["y"] << std::endl;

	Json j3("abc");
	j3.Append(123);
	std::cout << __func__ << ": j3 = " << j3 << std::endl;

	j3.Append(Json());
	std::cout << __func__ << ": j3 = " << j3 << std::endl;

	std::cout << __func__ << ": j3[0] = " << j3[0] << std::endl;
	std::cout << __func__ << ": j3[1] = " << j3[1] << std::endl;
	std::cout << __func__ << ": j3[2] = " << j3[2] << std::endl;

	Json j4;
	j4 += 123;
	j4 += 45;
	std::cout << __func__ << ": j4 = " << j4 << std::endl;

	Json j5;
	j5 += std::make_pair("key", 123);
	j5 += std::make_pair("second", Json().Append("a").Append("b"));
	std::cout << __func__ << ": j5 = " << j5 << std::endl;
}

int main()
{
	Json j;
	j["x"] = 123;
	j["y"] = "hello";
	j["z"][2] = true;
	std::cout << j << std::endl;

	test_constructors();
	test_assignments();
	test_array_and_object();
	return 0;
}
