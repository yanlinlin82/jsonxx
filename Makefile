all: demo

clean:
	@rm -fv demo

demo: demo.cpp json.hpp
	g++ -Wall -std=c++17 -g demo.cpp -o demo
	./demo
