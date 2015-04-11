#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>


int main(){
	std::unordered_set< int > set1;
	for( int i = 0 ; i < 1000000 ; ++i ){
		set1.insert( i );
	}
	for( int i = 0 ; i < 10 ; ++i ){
		int k;
		std::cin >> k;
		std::cout << k;
	}
	
}

