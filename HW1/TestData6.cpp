#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

#define nDatas 200

int main(){
	srand( time( 0 ) );
	for( int i = 0 ; i < nDatas ; ++i ){
		cout << rand() << " " << rand() << endl;
	}
}
