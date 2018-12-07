#include "json.hpp"

void test_constructors()
{
	Json j0;
	std::cout << __FUNCTION__ << ": j0 = " << j0 << std::endl;

	Json j1(false);
	std::cout << __FUNCTION__ << ": j1 = " << j1 << std::endl;

	Json j2(true);
	std::cout << __FUNCTION__ << ": j2 = " << j2 << std::endl;

	Json j3(0);
	std::cout << __FUNCTION__ << ": j3 = " << j3 << std::endl;

	Json j4(123);
	std::cout << __FUNCTION__ << ": j4 = " << j4 << std::endl;

	Json j5(-45);
	std::cout << __FUNCTION__ << ": j5 = " << j5 << std::endl;

	Json j6(1234567890123LL);
	std::cout << __FUNCTION__ << ": j6 = " << j6 << std::endl;

	Json j7(0.1f);
	std::cout << __FUNCTION__ << ": j7 = " << j7 << std::endl;

	Json j8(3.14159);
	std::cout << __FUNCTION__ << ": j8 = " << j8 << std::endl;

	Json j9("abc");
	std::cout << __FUNCTION__ << ": j9 = " << j9 << std::endl;

	Json j10(std::string("xyz"));
	std::cout << __FUNCTION__ << ": j10 = " << j10 << std::endl;
}

void test_assignments()
{
	Json j0 = Json();
	std::cout << __FUNCTION__ << ": j0 = " << j0 << std::endl;

	Json j1 = false;
	std::cout << __FUNCTION__ << ": j1 = " << j1 << std::endl;

	Json j2 = true;
	std::cout << __FUNCTION__ << ": j2 = " << j2 << std::endl;

	Json j3 = 0;
	std::cout << __FUNCTION__ << ": j3 = " << j3 << std::endl;

	Json j4 = 123;
	std::cout << __FUNCTION__ << ": j4 = " << j4 << std::endl;

	Json j5 = -45;
	std::cout << __FUNCTION__ << ": j5 = " << j5 << std::endl;

	Json j6 = 1234567890123LL;
	std::cout << __FUNCTION__ << ": j6 = " << j6 << std::endl;

	Json j7 = 0.1f;
	std::cout << __FUNCTION__ << ": j7 = " << j7 << std::endl;

	Json j8 = 3.14159;
	std::cout << __FUNCTION__ << ": j8 = " << j8 << std::endl;

	Json j9 = "abc";
	std::cout << __FUNCTION__ << ": j9 = " << j9 << std::endl;

	Json j10 = std::string("xyz");
	std::cout << __FUNCTION__ << ": j10 = " << j10 << std::endl;
}

void test_array_and_object()
{
	Json j0(Json().Append("abc"));
	std::cout << __FUNCTION__ << ": j0 = " << j0 << std::endl;

	Json j1(Json().Append("key", 123));
	std::cout << __FUNCTION__ << ": j1 = " << j1 << std::endl;
}

int main()
{
	test_constructors();
	test_assignments();
	test_array_and_object();
	return 0;
}
