CPPFLAGS = -O2 -std=c++11 -Wall
CPPFLAGS_DBG = -std=c++11 -Wall -g3
main:

calculator:evaluator.hpp evaluator.cpp calculator.cpp
	g++ $(CPPFLAGS) evaluator.cpp calculator.cpp -o calculator
debug:evaluator.hpp evaluator.cpp calculator.cpp
	g++ $(CPPFLAGS_DBG) evaluator.cpp calculator.cpp -o calculator
	gdb -q ./calculator	
