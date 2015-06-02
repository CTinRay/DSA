#include <iostream>

typedef double var;

inline var square( var x ){
	return x * x ;
}

inline bool inCircle( var x, var y , var r ){
	return ( square( x ) + square( y ) ) <= r ;
}

#define RADIUS 2
#define BOUND 50
#define PRECISE 10.0
int main(){
	for( int i = BOUND * PRECISE ; i >= -BOUND * PRECISE ; --i ){
		for( int j = BOUND * PRECISE ; j >= -BOUND * PRECISE ; --j ){
			var x = i / PRECISE ;
			var y = j / PRECISE ;
			std::cout << ( inCircle( x , y , RADIUS ) ? "+1" : "-1"  )
				  << " 1:" << x 
				  << " 2:" << y << std::endl;
		}
	}
}

				
