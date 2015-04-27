#include <iostream>
#include <string>
#include "evaluator.cpp"

int main(){
	std::string expression;
        Evaluator<double> eval;
	while( std::getline(std::cin, expression) ){
		std::cout << "RESULT: "<< eval.evaluate( expression ) << std::endl;		
	}
	return 0;
}
