all: demo

clean:
	@rm -fv demo.run demo.new.cpp

demo.new.cpp: demo.cpp
	./scripts/precomp.pl demo.cpp > demo.new.cpp

demo.out: demo.new.cpp json.hpp
	g++ -Wall -std=c++17 -g demo.new.cpp -o demo.out

demo: demo.out
	./scripts/check.pl demo.out demo.cpp
