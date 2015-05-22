#include <stdio.h>
#include <algorithm>
#include <iostream>
typedef int Int;
using namespace std;
#define MAX 100000000
Int GcdByReverserSearch( Int a, Int b, Int&times){
	for( Int i = min( a, b); i >= 1 ; --i ){
		times += 1;
		if( a % i == 0 && b % i == 0 ){
			return i;
		}
	}
}
Int GcdByFilter( Int a, Int b, Int&times ){
	for( Int i = 2; i <= min( a, b ); ++i ){
		times += 1;
		if( a % i == 0 && b % i == 0 ){
			return i * GcdByFilter( a/i , b/i, times);
		}
	}
	return 1;
}
Int GcdByFilterFasterInternal( Int a, Int b, Int s, Int&times ){
	for( Int i = s; i <= min( a, b) ; ++i ){
		times += 1;
		if( a % i == 0 && b % i == 0 ){
			return i * GcdByFilterFasterInternal( a / i , b / i , i, times );
		}
	}
	return 1;
}
Int GcdByFilterFaster( Int a , Int b, Int&times ){
	return GcdByFilterFasterInternal( a , b , 2, times );
}
Int GcdByBinary( Int a, Int b, Int&times ){
	Int m = max( a, b);
	Int n = min( a, b);
	Int ans = 1;
	while( n != 0 && m != 0 ){
		times += 1;
		if( n % 2 == 0 && m % 2 == 0 ){
			ans *= 2;
			n /= 2;
			m /= 2;
		}else if( n % 2 == 0 && m % 2 != 0 ){
			n /= 2;
		}else if( n % 2 != 0 && m % 2 == 0 ){
			m /= 2;
		}
		if( n > m ){
			Int tmp = m;
			m = n;
			n = tmp;
		}
		m = m - n;
	}
	return n * ans;
}
Int GcdByEuclid( Int a, Int b, Int&times ){
	Int m = max( a, b);
	Int n = min( a, b);
	while( m % n != 0 ){
		times += 1;
		Int tmp = n;
		n = m % n;
		m = tmp;
	}
	return n;
}
void CallGcd( Int(*gcdFunc)(Int , Int, Int&), Int a, Int b, const char*funcName ){
	static int maxTimes = 0;
	static int maxCase = 0;
	
	Int times = 0;
	
	Int ans = (*gcdFunc)( a, b, times);
	if( times > maxTimes ){
		maxCase = b;
		maxTimes = times;
	}
		
	cout << "Case (" << a << ", " << b << "): "
	     << times << " max: " << maxTimes << " case: " << maxCase  << endl;
}
int main(){
	Int a, b;
	cout << "i" << "\t" << "maxIteration" << "\t" << "maxCaseJ" << "\t" << "maxCaseGcd" << endl; 
	for( int i = 1 ; i < MAX ; ++ i ){
		int maxTimes = 0;
		int maxCaseGcd = 0;
		int maxCaseJ = 0;
		for( int j = 0 ; j < i ; ++j ){
			int times = 0;
			int gcd = GcdByBinary( i , j, times );
			if( times > maxTimes ){
				maxTimes = times;
				maxCaseGcd = gcd;
				maxCaseJ = j;
			}
		}
		cout << i << "\t\t" << maxTimes << "\t\t" << maxCaseJ << "\t\t" << maxCaseGcd << endl; 
					
	}
	return 0;
}
