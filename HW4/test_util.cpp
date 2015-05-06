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
			std::cout << itMap -> first << ":" << itMap -> second << " " ; 
		}
		std::cout << std::endl;
	}
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
		
