all: test

clean:
	@rm -fv test

test: test.cpp json.hpp
	g++ -Wall -std=c++17 test.cpp -o test
	./test
