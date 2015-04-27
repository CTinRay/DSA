#include <functional>
#include <string>
#include <map>
#include <stack>
#include <vector>
typedef unsigned int uint;
typedef float Float;

//namespace evaluate{

template<typename NumberType>
class Evaluator{
public:
	Evaluator(); 
	struct Operate{
		int precedence;
		std::string face;
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
	void calculate( std::vector<NumberType>&, std::vector<Operate>&, int, std::string& );					       
	//friend std::ostream& operator << (std::ostream&, const Operate&);
	void printOperates(std::vector<Operate>&);

};	


template<class C>
std::ostream& operator << (std::ostream&, const std::vector<C>&);

//}
		
	
