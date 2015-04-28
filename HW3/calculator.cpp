#include <iostream>
#include <iomanip> 
#include <string>
#include "evaluator.cpp"

int main(){
	std::string expression;

	#ifdef _INTEGER
        Evaluator<int> eval;
	#endif

	#ifdef _SCIENTIFIC
        Evaluator<double> eval;	
	#endif


	while( std::getline(std::cin, expression) ){

		#ifdef _INTEGER
		std::cout << "RESULT: "<< eval.evaluate( expression ) << std::endl;		
		#endif

		#ifdef _SCIENTIFIC
		std::cout << std::fixed;
		std::cout << "RESULT: "<<  std::setprecision(6) << eval.evaluate( expression ) << std::endl;		
		#endif
		
	}
	return 0;
}
