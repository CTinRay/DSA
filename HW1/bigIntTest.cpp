#include "biginteger.h"
#include <iostream>
#include <string>
using namespace std;
#define test2  1000000000
#define test3 "1073741824"
#define RIGHT 1
#define LEFT -1

void shiftTest( int way , BigInteger&bigInt , int times = expo2 ){
	cout << "#####Shift Test Start#####" << endl;
	for( int i = 0 ; i < times ; ++i ){
		if( way == RIGHT ){
			bigInt.shiftRight(1);
			cout << "Shift Right " << i << " times:" << endl;
			bigInt.printSets();
			cout << ".........." << endl;
		}

		if( way == LEFT ){
			bigInt.shiftLeft(1);
			cout << "Shift Left " << i << " times:" << endl;
			bigInt.printSets();
			cout << ".........." << endl;
		}

	}
	cout << "##### Shift Test End #####" << endl;
}

BigInteger modTest( BigInteger &a, BigInteger &b, string nameA = "A" , string nameB = "B" ){
	cout << "##### Mod Test Start #####" << endl;
	BigInteger result;
	result = a % b;
	cout << nameA << " % " << nameB << ":" << endl;
	result.printSets();
	cout << "##### Mod Test End #####" << endl;
	return result;
}
	
void divideTest( BigInteger& bigInt , int divisor , int times = 3 , string name = "" ){
	cout << "##### Divide /= Test Start" << endl;
	for( int i = 0 ; i < times ; ++i ){
		cout << name << " /= " << divisor << " for " << i << " times:" << endl;
		bigInt /= divisor;
		bigInt.printSets();
		cout << "........." << endl;
	}
	cout << "##### Divide /= Test End" << endl;
}

int main(){

	/*	BigInteger bigInt1;
	cout << "bigInt1:" << endl;
	bigInt1.printSets();
	*/		
	BigInteger bigInt2(test2);
	cout << "bigInt2( " << test2 << " ):" << endl;
	bigInt2.printSets();	
       
	BigInteger bigInt3(test3);
	cout << "bigInt3(\" " << test3 << " \")" << endl;
	bigInt3.printSets();	        	
	
	//divideTest( bigInt3 , 1000000000 , 3 , "bigInt3" );
	modTest( bigInt3 , bigInt2 );

	cout << "cout << bigInt3:" << endl;
	cout << bigInt3;
	

	cout << endl 
	     << "-----End of Test-----" << endl;
}

	
		

