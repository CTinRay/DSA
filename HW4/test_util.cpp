#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "test_util.hpp"

void printVec( std::vector<Example*>vec ){
	for( auto itVec = vec.begin() ; 
	     itVec != vec.end(); ++ itVec ){
		std::cout << (*itVec) -> decision << " ";
		for( auto itMap = (*itVec) -> attrs . begin() ; 
		     itMap != (*itVec) -> attrs . end(); ++itMap ){
			//std::cout << itMap -> first << ":" << itMap -> second << " " ; 
		}
		std::cout << std::endl;
	}
}

int printOrder(std::map<Attr,std::vector<Example*> >orderByAttr){
	for( auto itMap = orderByAttr.begin() ;
	     itMap != orderByAttr.end(); ++ itMap ){
		Attr attr = itMap -> first ;
		std:: cout << "attr:" << attr << std::endl;
		auto vec = itMap -> second;
		for( unsigned int i = 0 ; i < vec.size() ; ++ i ){
			std::cout << vec[ i ] -> decision << ":" << vec[ i ] -> attrs[ attr ]  << std::endl;
		}
		std::cout << std::endl;
	}
	return 0;
}


#define INDENT "   "
void printTree( Node*node, std::string indent = "" ){	
	if( node -> decision == 0 ){
		std::cout << indent << "attribute: " << node -> attr << " threshold: " << node -> threshold << std::endl;
		std::cout << "l";
		printTree( node -> left , indent + INDENT );
		std::cout << "r";
		printTree( node -> right , indent + INDENT );
	}else{
		std::cout << indent << "decision: " << node -> decision << std::endl;
	}
}

void drawLine( double fX , double fY, double tX, double tY ){
	std::cout << "     \\draw[very thick,gray] ("  
		  << fX << "," << fY << ") -- (" << tX << "," << tY << ");" << std::endl;
}
		
void printGraph( Node*node, double startX, double startY, double endX, double endY ){

	if( node -> decision != 0 ){
		return ;
	}
	if( node -> attr == 1 ){
		drawLine( node -> threshold , startY , node -> threshold , endY );
		printGraph( node -> left , startX , startY , node -> threshold , endY );
		printGraph( node -> right , node -> threshold , startY , endX , endY );
	}else{
		drawLine( startX , node -> threshold , endX , node -> threshold );
		printGraph( node -> left , startX , node -> threshold , endX , endY );
		printGraph( node -> right , startX , startY , endX , node -> threshold );
	}
}
