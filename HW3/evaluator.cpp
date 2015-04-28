#include <sstream>
#include <iostream>
#include "evaluator.hpp"
#include <math.h>
#include <string>
#include <iomanip> 
//using namespace evaluate;
typedef long long int lli;
typedef int Int;
#define MaxPrecedence 20
template<typename T>
Evaluator<T>::Evaluator(){

	binOpMap["*"].precedence = 5;
	binOpMap["*"].face = " *";
	binOpMap["*"].evaluate = ([](std::vector<T>&nums){ 
	        T first = nums.back(); nums.pop_back();
	        T second = nums.back(); nums.pop_back();
		nums.push_back(first * second);
		});

	binOpMap["/"].precedence = 5;
	binOpMap["/"].face = " /";
	binOpMap["/"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
	        T second = nums.back(); nums.pop_back();
		nums.push_back(second / first);
	};
				
	binOpMap["%"].precedence = 5;
	binOpMap["%"].face = " %";
	binOpMap["%"].evaluate = [](std::vector<T>&nums){ 
	        int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(second % first);
	};
		
	binOpMap["+"].precedence = 6;
	binOpMap["+"].face = " +";
	binOpMap["+"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back( second + first );
	};

	binOpMap["-"].precedence = 6;
	binOpMap["-"].face = " -";
	binOpMap["-"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
		nums.push_back(second - first);
	};

	binOpMap[">>"].precedence = 7;
	binOpMap[">>"].face = " >>";
	binOpMap[">>"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(second >> first);
	};

	binOpMap["<<"].precedence = 7;
	binOpMap["<<"].face = " <<";
	binOpMap["<<"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(second << first);
	};


	binOpMap["&"].precedence = 10;
	binOpMap["&"].face = " &";
	binOpMap["&"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(first & second);
	};

	binOpMap["^"].precedence = 11;
	binOpMap["^"].face = " ^";
	binOpMap["^"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(first ^ second);
	};

	binOpMap["|"].precedence = 12;
	binOpMap["|"].face = " |";
	binOpMap["|"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(first | second);
	};

	binOpMap["&&"].precedence = 13;
	binOpMap["&&"].face = " &&";
	binOpMap["&&"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back(first && second);
	};

	binOpMap["||"].precedence = 14;
	binOpMap["||"].face = " ||";
	binOpMap["||"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back(first || second);
	};
	
	binOpMap[","].precedence = MaxPrecedence - 1;
	binOpMap[","].face = "";
	binOpMap[","].evaluate = [](std::vector<T>&nums){ 
	};

	binOpMap[")"].precedence = 2;
	binOpMap[")"].face = " )";
	binOpMap[")"].evaluate = [](std::vector<T>&nums){
	};

	unrOpMap["("].precedence = 2;
	unrOpMap["("].face = " (";
	unrOpMap["("].evaluate = [](std::vector<T>&nums){
	};


	unrOpMap["+"].precedence = 3;
	unrOpMap["+"].face = " +";
	unrOpMap["+"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( + first );
	};

	unrOpMap["-"].precedence = 3;
	unrOpMap["-"].face = " -";
	unrOpMap["-"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( - first );
	};


	unrOpMap["!"].precedence = 3;
	unrOpMap["!"].face = " !";
	unrOpMap["!"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( ! first );
	};


	unrOpMap["~"].precedence = 3;
	unrOpMap["~"].face = " ~";
	unrOpMap["~"].evaluate = [](std::vector<T>&nums){
		Int first = nums.back(); nums.pop_back();
		nums.push_back( ~ first );
	};


	funcMap["sin"].precedence = 1;
        funcMap["sin"].face = " sin";
        funcMap["sin"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( sin( first ) );
	};

	funcMap["cos"].precedence = 1;
        funcMap["cos"].face = " cos";
        funcMap["cos"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( cos( first ) );
	};

	funcMap["exp"].precedence = 1;
        funcMap["exp"].face = " exp";
        funcMap["exp"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( exp( first ) );
	};


	funcMap["log"].precedence = 1;
        funcMap["log"].face = " log";
        funcMap["log"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( log( first ) );
	};

	funcMap["pow"].precedence = 0;
        funcMap["pow"].face = " pow";
        funcMap["pow"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
		nums.push_back( pow( second, first ) );
	};

	funcMap["sqrt"].precedence = 1;
        funcMap["sqrt"].face = " sqrt";
        funcMap["sqrt"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( sqrt( first ) );
	};

	funcMap["fabs"].precedence = 1;
        funcMap["fabs"].face = " fabs";
        funcMap["fabs"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( fabs( first ) );
	};


}



template<typename T>
T Evaluator<T>::evaluate(std::string expression){
	std::vector<std::string>tockens;
	std::vector<T>nums;//Emulate stack with vector ;
	std::vector<Operate>operates;//Emulate stack with vector
	std::string postfix;
	bool expectBinOp = false;
	int precedenceOffset = 0;
	tockenize( expression, tockens);

	//!!DEBUG
	//std::cout << "tockens:" << tockens << std::endl;
	for( int i = 0 ; i < tockens.size() ; ++i ){
		// typename  std::map<std::string,Evaluator<T>::Operate>::iterator opIterator;
		if( tockens[i] == "(" ){
			precedenceOffset -= MaxPrecedence;
		        expectBinOp = false;
			continue;
		}
		else if( tockens[i] == ")" ){
			precedenceOffset += MaxPrecedence;
		        expectBinOp = true;
			continue;
		}
		else if( isNum( tockens[i] ) ){			
			T num = toNum( tockens[ i ] );
			nums.push_back( num );
			#ifdef _INTEGER
			postfix.append( " " + tockens[i] );
			#endif

			#ifdef _SCIENTIFIC
			postfix.append( " " + toStr( num ) );
			#endif

			expectBinOp = true;
		}
		else{
			Operate operate;
			if( expectBinOp ){
				if( binOpMap.count( tockens[ i ] ) == 0  ){
					std::cout << "error: unexpected tocken: " << tockens[ i ] << ", at " << i << std::endl
						  << "expect bunary operator" << std::endl << std::endl;
					return -1;
				}	
				operate = binOpMap.at( tockens[ i ] );
				expectBinOp = false;
				operate.precedence += precedenceOffset;
				calculate( nums, operates , operate.precedence , postfix );			
			}
			else{
				if( unrOpMap.count( tockens[ i ] ) == 0 &&
				    funcMap.count( tockens[ i ] ) == 0  ){
					std::cout << "error: unexpected tocken: " << tockens[ i ] << ", at " << i << std::endl
						  << "expect unary operator or function" << std::endl << std::endl;
					return -1;
				}			
				operate = unrOpMap.count( tockens[ i ] ) ? unrOpMap.at( tockens[i] ) : funcMap.at( tockens[i] ) ;
				expectBinOp = false;
				operate.precedence += precedenceOffset;
			}
			operates.push_back( operate );			

		}
		std::cout << "Processing token: " << i  << std::endl;
		std::cout << "Generated postfix: " << postfix << std::endl;
		std::cout << "Operator stack: " ;
		printOperates( operates );
		std::cout << std::endl << "=====================" << std::endl;

	}
	calculate( nums, operates, 2147483647 , postfix );
	//output postfix
	std::cout << "Postfix Exp:" << postfix << std::endl;
	
	
	return nums[0];
}

template<typename T>
void Evaluator<T>::calculate( std::vector<T>&nums, std::vector<Operate>&operates, int precedence, std::string&postfix ){
	while( !operates.empty() &&
	        operates.back().precedence <= precedence ){
		//!!DEBUG
		// std::cout << "Here" << std::endl;
		//std::cout << opIterators.back() << "-" << opIterators.back() -> second . precedence << std::endl;
		postfix.append( operates.back().face );
		operates.back().evaluate( nums );
	        operates.pop_back();
	}
}

template<typename T>
void Evaluator<T>::tockenize(const std::string str,std::vector<std::string>&tockens)const{
	int strLength = str.length();
	int i = 0;
	while( i < strLength ){
		int start = i;
		int&end = i;
		if( str[ start ] == ' ' ){
			++i;
		}
		else if( isalpha( str[ start ] ) ){
			while( isalpha( str[ end ] ) && end < strLength ){
				end += 1;
			}
			tockens.push_back( str.substr( start , end - start ) );
		}
		else if( isdigit( (char)str[ start ] ) || str[ start ] == '.' ){
			while( end < strLength && ( isdigit( str[ end ] ) || str[ end ] == '.' )  ){
				end += 1;
			}

			tockens.push_back( str.substr( start , end - start ) );
		}
		else{
			std::string buffer;
			buffer.push_back( str[ end ] );
			while( !unrOpMap.count( buffer ) &&
			       !binOpMap.count( buffer ) ){
				buffer.push_back( str[ ++end ] );				
			}
			while( unrOpMap.count( buffer ) ||
			       binOpMap.count( buffer ) ){
				buffer.push_back( str[ ++end ] );
			}
			tockens.push_back( str.substr( start , end - start ) );			
		}
	}
}



template<typename T>
std::ostream& operator << (std::ostream&out,typename Evaluator<double>::Operate &a){
	out << a.face ;
	return out;
}



template<typename C>
std::ostream& operator << (std::ostream &out, const std::vector<C>&vec){
	for( int i = 0 ; i < vec.size() - 1 ; ++i ){
		std::cout << vec[i] << ", ";
	}
	std::cout << vec[ vec.size() - 1 ] ;
	return out;
}


template<typename T>
T Evaluator<T>::toNum( std::string str ){
	std::stringstream ss( str );
        T num;
	ss >> num;
	return num;
}


template<typename T>
std::string Evaluator<T>::toStr( T num ){
	std::stringstream ss;
	ss << std::fixed << std::setprecision( 6 ) << num;
	return ss.str();
}


template<typename T>
bool Evaluator<T>::isNum( std::string str ){
	if( isdigit( str[0] ) || str[0] == '.' ) return true;
	return false;
}

template<typename T>
void Evaluator<T>::printOperates(std::vector<Operate>&vec){
	for( int i = 0 ; i < vec.size()  ; ++i ){
		std::cout << vec[ i ].face ;
	}

}
