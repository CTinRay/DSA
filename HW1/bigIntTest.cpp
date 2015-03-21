#include "biginteger.h"
#include <iostream>
#include <string>
using namespace std;
#define test2  1000000000
#define test3 "43466557686937456435688527675040625802564660517371780402481729089536555417949051890403879840079255169295922593080322634775209689623239873322471161642996440906533187938298969649928516003704476137795166849228875"
#define RIGHT 1
#define LEFT -1
#define fib1200 "27269884455406270157991615313642198705000779992917725821180502894974726476373026809482509284562310031170172380127627214493597616743856443016039972205847405917634660750474914561879656763268658528092195715626073248224067794253809132219056382939163918400"
#define fib1199 "16853715462723491170192455155474509439735508098002844482595024267242834241944568802972062546036647271061368074350126366761024314387612704997681075881249068173708819887547926782036364656854243643357321405009295781778073579513653790357382166526579640801"

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

BigInteger modTest(){
	string strA = "1018517988167243043134222844204689080525734196832968125318070224";
	string strB = "10";
	BigInteger a(strA);
	BigInteger b( set10Max );
		
	cout << "##### Mod Test Start #####" << endl;
	BigInteger result;
	cout << "a:" << endl;
	a.printSets();
	cout << ".........." << endl;
	result = a % b;
	cout << strA << " % " << set10Max << ":" << endl;
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

void divide10Test(){
	int times = 8;
	string strA = "1018517988167243043134222844204689080525734196832968125318070224677190649881668353091698688";
	BigInteger a(strA);
	int b = set10Max;
	cout << "##### Divide /= Test Start" << endl;
	a.printSets();
	for( int i = 0 ; i < times ; ++i ){
		cout << strA << " /= " << set10Max << " for " << i << " times:" << endl;
		a /= b;
		cout << a << endl;
		a.printSets();		
		cout << "........." << endl;
	}
	cout << "##### Divide /= Test End" << endl;

}
void divideTest(){
	cout << "##### Divide /= Test Start" << endl;
	BigInteger bigInt("2037035976334486086268445688409378161051468393665936250636140449354381299763336706183397376");
	cout << "2037035976334486086268445688409378161051468393665936250636140449354381299763336706183397376 : " << endl;
	bigInt.printSets();
	for( int i = 300 ; i >= 1 ; --i ){
		cout << "2^" << i << ":\t" << bigInt << endl;
		bigInt /= 2;
	}
	bigInt.printSets();
}
	

void constructAndPrintTest(){
	string tmp;
	cin >> tmp;
	while( tmp != "exit" ){
		BigInteger bigInt( tmp );
		cout << bigInt << endl;
		cin >> tmp;
	}
}

void shiftAndPrintTest(){
	BigInteger bigInt(1);
	for( int i = 0 ; i < 300 ; ++i ){
		cout << "2^" << i << ":\t" << bigInt << endl;
		bigInt.shiftLeft(1);
	}
	bigInt.printSets();
}

void addAndPrintTest(){


	BigInteger bigInt(1);
	for( int i = 0 ; i < 300 ; ++i ){
		cout << "sign: " << bigInt.sign << " 2^" << i << ":\t" << bigInt << endl;
		bigInt = bigInt.AbsAdd( bigInt , bigInt );
	}
	bigInt.printSets();
}

void mulAndPrintTest(){

	BigInteger bigInt(1);
	for( int i = 0 ; i < 30 ; ++i ){
		cout << " 2^" << i << ":\t" << bigInt << endl;
		bigInt = bigInt *= 2;
	}
	bigInt.printSets();
}

string primeTable =
	{"22801761907",
	 "22801762441"
	};

void GcdByEuclid(){
	BigInteger a("22801761907");
	BigInteger b("22801762441");
	BigInteger m = a > b ? a : b;
	BigInteger n = a < b ? a : b;
	cout << "m:" << endl;
	m.printSets();
	cout << "n:" << endl;
	n.printSets();
	BigInteger zero( 0 );
	BigInteger tmp ;
	while( ( m % n ) > zero ){	
		cout << tmp << endl;
		tmp = n;
		n = m % n;
		m = tmp;
	}

	cout << "Ans:" << endl;
	cout << n << endl;
	n.printSets();


}


void fibonacci(){
	string strA = "222232244629420445529739893461909967206666939096499764990979600";
	string strB = "137347080577163115432025771710279131845700275212767467264610201";
	BigInteger a(fib1200);
	BigInteger b(fib1199);
	BigInteger tmp;
	for( int i = 1198 ; i >= 1 ; --i ){
		tmp = a % b;
		cout << "Fibo " << i << ":" << tmp << endl;
		a = b;
		b = tmp;
	}
	tmp.printSets();

}

void subTest(){
	string strA = "222232244629420445529739893461909967206666939096499764990979600";
	string strB = "137347080577163115432025771710279131845700275212767467264610201";
	BigInteger a(strA);
	cout << "a:" << strA << endl;
	a.printSets();
	BigInteger b(strB);
	cout << "b:" << strB << endl;
	b.printSets();
	
	BigInteger c =  a - b;
	cout << "c:" << endl;
	c.printSets();
	cout << c << endl;

}

void outPutTest(){
	string strA = "1018517988167243043134222844204689080525734196832968125318070224677190649881668353091698688";
	BigInteger a( strA );
	cout << strA << ":" << endl;
	cout << a ;
	a.printSets();	
}
	
int main(){
	//shiftAndPrintTest();
	//addAndPrintTest();
	//mulAndPrintTest();
	//fibonacci();
	//divideTest();
	//subTest();
	//GcdByEuclid();
	//modTest();
	//divide10Test();
	//outPutTest();
	constructAndPrintTest();
	cout << endl 

	     << "-----End of Test-----" << endl;
}

	
		

