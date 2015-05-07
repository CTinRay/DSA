
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
//#include "tree_generator.hpp"
#include "test_util.hpp"
#include <sstream>

int main(int argc, char**argv ){
	std::fstream dataFileFS;
	dataFileFS.open( argv[1] , std::fstream::in );


#ifdef _DEBUG
	if( ! dataFileFS.is_open() ){
		std::cout << "Open file fail!!" << std::endl;
	}else{

		std::cout << "Open success!!" << std::endl;

	}
#endif

	double epsilon = atof( argv[2] );
	std::string buffer;
	std::vector<Example*>examples;
	std::set<Attr>attrSet;

	while( std::getline( dataFileFS, buffer ) ){		
		Example*example = new Example;
		std::stringstream ss( buffer );
	        ss >> example -> decision;		
		Attr attr;
		ss >> attr;
		while( ss.good() ){	
			attrSet.insert( attr );
			char cbuffer;
			ss >> cbuffer;
			double tmp;
			ss >> tmp;
			example -> attrs[attr] = tmp;
			ss >> attr ;
		}
		examples.push_back( example );
		
	}
	#ifdef _DEBUG
	//printVec( examples );
	#endif

	Node*root = buildTree( examples ,attrSet, epsilon );
	#ifdef _DEBUG
	printTree( root , "   " );
	#endif

	printTreeInC( root );
	
	return 0;
}
