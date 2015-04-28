CPPFLAGS = -O2 -std=c++11 -Wall 
CPPFLAGS_DBG = -std=c++11 -Wall -g3
main:evaluator.hpp evaluator.cpp calculator.cpp
	g++ $(CPPFLAGS) -D_INTEGER evaluator.cpp calculator.cpp -o 3_4_1.out	
	g++ $(CPPFLAGS) -D_SCIENTIFIC evaluator.cpp calculator.cpp -o 3_4_2.out
calculator:evaluator.hpp evaluator.cpp calculator.cpp -o 
	g++ $(CPPFLAGS) evaluator.cpp calculator.cpp -o calculator
debug:evaluator.hpp evaluator.cpp calculator.cpp
	g++ $(CPPFLAGS_DBG) evaluator.cpp calculator.cpp -o calculator
	gdb -q ./calculator	
