#include "biginteger.h"
#include <iostream>
using namespace std;
#define test2  1000000000
#define test3 "1073741824"


int main(){

	/*	BigInteger bigInt1;
	cout << "bigInt1:" << endl;
	bigInt1.printSets();
	*/		
	BigInteger bigInt2(test2);
	cout << "bigInt2( " << test2 << " ):" << endl;
	bigInt2.shiftLeft(30);
	bigInt2.printSets();	
       
	BigInteger bigInt3(test3);
	cout << "bigInt3(\" " << test3 << " \")" << endl;
	bigInt3.printSets();
	
	
	
	for( int i = 0 ; i < 30 ; ++i ){
		bigInt2.shiftRight( 1 );
		cout << "bigInt2 >> " << 1 << ":" << endl;
		bigInt2.printSets();
	}
	
	
	bigInt3 = bigInt3 % bigInt2;
	cout << "bigInt3 = bigInt3 % bigInt2:" << endl;
	bigInt3.printSets();


	cout << endl 
	     << "-----End of Test-----" << endl;
}

	
		

