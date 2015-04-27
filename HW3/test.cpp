#include <functional>
#include <string>
#include <map>
#include <stack>
#include <vector>
typedef int Int;
typedef float Float;

#include <sstream>
#include <iostream>

//using namespace evaluate;
typedef long long int lli;




//namespace evaluate{

	template<typename NumberType>
	class Evaluator{
	public:
		Evaluator(); 
		struct Operate{
			bool print;
			int precedence;
			std::function<void (std::vector<NumberType>&)>evaluate;
		};
		typedef typename std::map<std::string,Evaluator<NumberType>::Operate>::iterator OpIterator;
		NumberType evaluate(std::string);
	private:
	         std::map<std::string,Operate>unrOpMap;		
		 std::map<std::string,Operate>binOpMap;		
		 std::map<std::string,Operate>funcMap;
		void tockenize(const std::string, std::vector<std::string>&)const;
		bool isNum( std::string );
		NumberType toNum( std::string );
		void calculate( std::vector<NumberType>&, std::vector<OpIterator>&, int, std::string& );
	};	

	template<class C>
	std::ostream& operator << (std::ostream&, const std::vector<C>&);
	template<typename T>
	std::ostream& operator << (std::ostream&, const std::map<std::string,typename Evaluator<T>::Operate>&a);
//}
		
	


template<typename T>
Evaluator<T>::Evaluator(){

	binOpMap["*"].precedence = 5;
	binOpMap["*"].print = true;
	binOpMap["*"].evaluate = ([](std::vector<T>&nums){ 
	        T first = nums.back(); nums.pop_back();
	        T second = nums.back(); nums.pop_back();
		nums.push_back(first * second);
		});

	binOpMap["/"].precedence = 5;
	binOpMap["/"].print = true;
	binOpMap["/"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
	        T second = nums.back(); nums.pop_back();
		nums.push_back(first / second);
	};
				
	binOpMap["%"].precedence = 5;
	binOpMap["%"].print = true;
	binOpMap["%"].evaluate = [](std::vector<T>&nums){ 
	        int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(first % second);
	};
		
	binOpMap["+"].precedence = 6;
	binOpMap["+"].print = true;
	binOpMap["+"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back( first + second );
	};

	binOpMap["-"].precedence = 6;
	binOpMap["-"].print = true;
	binOpMap["-"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
		nums.push_back(first + second);
	};

	binOpMap[">>"].precedence = 7;
	binOpMap[">>"].print = true;
	binOpMap[">>"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(first >> second);
	};

	binOpMap["<<"].precedence = 7;
	binOpMap["<<"].print = true;
	binOpMap["<<"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(first << second);
	};


	binOpMap["&"].precedence = 10;
	binOpMap["&"].print = true;
	binOpMap["&"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(first & second);
	};

	binOpMap["^"].precedence = 11;
	binOpMap["^"].print = true;
	binOpMap["^"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
		nums.push_back(first ^ second);
	};

	binOpMap["|"].precedence = 12;
	binOpMap["|"].print = true;
	binOpMap["|"].evaluate = [](std::vector<T>&nums){ 
		int first = nums.back(); nums.pop_back();
		int second = nums.back(); nums.pop_back();
	        nums.push_back(first | second);
	};

	binOpMap["&&"].precedence = 13;
	binOpMap["&&"].print = true;
	binOpMap["&&"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back(first && second);
	};

	binOpMap["||"].precedence = 14;
	binOpMap["||"].print = true;
	binOpMap["||"].evaluate = [](std::vector<T>&nums){ 
		T first = nums.back(); nums.pop_back();
		T second = nums.back(); nums.pop_back();
	        nums.push_back(first || second);
	};
	
	binOpMap[","].precedence = 2;
	binOpMap[","].print = false;
	binOpMap[","].evaluate = [](std::vector<T>&nums){ 
	};

	unrOpMap[")"].precedence = 2;
	unrOpMap[")"].print = true;
	unrOpMap[")"].evaluate = [](std::vector<T>&nums){
	};

	unrOpMap["("].precedence = 2;
	unrOpMap["("].print = false;
	unrOpMap["("].evaluate = [](std::vector<T>&nums){
	};


	unrOpMap["+"].precedence = 3;
	unrOpMap["+"].print = true;
	unrOpMap["+"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( + first );
	};

	unrOpMap["-"].precedence = 3;
	unrOpMap["-"].print = true;
	unrOpMap["-"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( - first );
	};

	unrOpMap["!"].precedence = 3;
	unrOpMap["!"].print = true;
	unrOpMap["!"].evaluate = [](std::vector<T>&nums){
		T first = nums.back(); nums.pop_back();
		nums.push_back( ! first );
	};
}



template<typename T>
T Evaluator<T>::evaluate(std::string expression){
	std::vector<std::string>tockens;
	std::vector<T>nums;//Emulate stack with vector ;
	std::vector<OpIterator>operates;//Emulate stack with vector
	std::string postfix;
	bool expectBinOp = false;

	tockenize( expression, tockens);

	for( int i = 0 ; i < tockens.size() ; ++i ){
		typename  std::map<std::string,Evaluator<T>::Operate>::iterator opIterator;
		if( isNum( tockens[i] ) ){			
			nums.push_back( toNum( tockens[ i ] ) );
			postfix.append( tockens[i] );
			expectBinOp = true;
		}
		else{
			if( expectBinOp ){
				if( ( opIterator = binOpMap.find( tockens[ i ] )) == binOpMap.end() ){
					std::cout << "error: unexpected tocken: " << tockens[ i ] << ", at " << i << std::endl
						  << "expect binary operator" << std::endl << std::endl;
					return -1;
				}			
				expectBinOp = false;
			}
			else{
				if( (opIterator = unrOpMap.find( tockens[ i ] )) == unrOpMap.end() &&
				    (opIterator = funcMap.find( tockens[ i ] )) == funcMap.end() ){
					std::cout << "error: unexpected tocken: " << tockens[ i ] << ", at " << i << std::endl
						  << "expect binary operator" << std::endl << std::endl;
					return -1;
				}			
				expectBinOp = false;
				operates.push_back( opIterator );
			}
			calculate( nums, operates, opIterator -> second . precedence, postfix );
		}
	}
	calculate( nums, operates, -1, postfix );

	//output postfix
	std::cout << "Postfix Exp: " << postfix << std::endl;
	
	return nums[0];
}

template<typename T>
void Evaluator<T>::calculate( std::vector<T>&nums, std::vector<OpIterator>&opIterators, int precedence, std::string&postfix ){
	while( opIterators[ opIterators.size() - 1 ] -> second . precedence <= precedence ){
		if( opIterators[ opIterators.size() - 1 ] -> second . print ){
			postfix.append( opIterators[ opIterators.size() - 1 ] -> first );
		}
		opIterators[ opIterators.size() - 1 ] -> second . evaluate( nums );
		opIterators.pop_back();
	}
}

template<typename T>
void Evaluator<T>::tockenize(const std::string str,std::vector<std::string>&tockens)const{
	int strLength = str.length();
	for( int i = 0 ; i < strLength ; ++i ){
		int start = i;
		int&end = i;
		if( isalpha( str[ start ] ) ){
			while( isalpha( str[ end ] ) && end < strLength ){
				end += 1;
			}
			tockens.push_back( str.substr( start , end - start ) );
		}
		else if( isdigit( (char)str[ start ] ) ){
			while( isdigit( str[ end ] ) && end < strLength ){
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
std::ostream& operator << (std::ostream&out, const std::map<std::string,typename Evaluator<T>::Operate>&a){
	if( a -> second.print ){
		out << a -> first << std::endl;
	}
	return out;
}



template<typename C>
std::ostream& operator << (std::ostream &out, const std::vector<C>&vec){
	for( int i = 0 ; i < vec.size() - 1 ; ++i ){
		std::cout << vec[i] << ", ";
	}
	std::cout << vec[ vec.size() - 1 ] << std::endl;
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
bool Evaluator<T>::isNum( std::string str ){
	if( isdigit( str[0] ) ) return true;
	return false;
}



int main(){
	Evaluator<double> a;
	a.evaluate("1+1");
	return 0;
}
	
	
