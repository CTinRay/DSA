#include "biginteger.h"
#include <iostream>
using namespace std;
#define test2 123456789
#define test2Mul 10
#define test3 "123456789"


int main(){

	BigInteger bigInt1;
	cout << "bigInt1:" << endl;
	bigInt1.printSets();
	
	BigInteger bigInt2(test2);
	cout << "bigInt2( " << test2 << " ):" << endl;
	bigInt2.printSets();
	
	bigInt2 *= test2Mul;
	cout << "bigInt2*=" << test2Mul << endl;
	bigInt2.printSets();


	

	BigInteger bigInt3(test3);
	cout << "bigInt3(\" " << test3 << " \")" << endl;
	bigInt3.printSets();

	
	cout << endl 
	     << "-----End of Test-----" << endl;
}

	
		

