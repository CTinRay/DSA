#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "biginteger.h"

#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

#define N_DATA 1000
#define SEGMAX 10000
#define SEGLEN 4
#define NSEGSMAX 64

string randString( int nSegs = rand() % NSEGSMAX ){ // strlen = nSegs * 4
	stringstream ss;
	ss << rand() % SEGMAX;
	ss << setfill('0');
	
	for( int i = 0 ; i < nSegs - 1 ; ++ i ){
		ss << setw(4) << rand() % SEGMAX;
	}
	return ss.str();
}

int main(){
	string testFileName = "7in";
	string ansFileName = "7ans";
	srand( time( 0 ) );

	ofstream testFile;
	testFile.open(testFileName);

	for( int i = 0 ; i < N_DATA ; ++i ){
		string str1 = randString();
		string str2 = randString();
		string str3 = randString();
		BigInteger a( str1 );
		BigInteger b( str2 );
		BigInteger m( str3 );

		stringstream ss;
		ss << ( a * m ) << " " << ( b * m ) << endl;

		string strOut = ss.str();
		cout << strOut;
		//testFile << strOut;
		//cout << m << endl;
	}
	testFile.close();
	
	return 0;
}

		
