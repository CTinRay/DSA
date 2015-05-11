#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
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

	int nTree = atoi( argv[2] );
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

	uint nExamples = examples.size();
	uint subSetSize = ( nExamples + 2 ) / 3 ;
	std::vector<Node*>forestRoots(nTree);
	srand (time(NULL));	
#ifdef _DEBUG
	std::cout << nExamples << std::endl;
#endif


	for( int i = 0 ; i < nTree ; ++i ){
		std::vector<Example*>subSet(subSetSize);
		for( uint j = 0 ; j < subSetSize ; ++j ){
			uint randInd = rand() % ( nExamples - j );		
			#ifdef _DEBUG
			
			#endif
			subSet[ j ] = examples[ randInd ];
			std::swap( examples[ nExamples - j -1 ] , examples[ randInd ] );
		}
#ifdef _DEBUG
		printVec( examples );
#endif
		forestRoots[i] = buildTree( subSet ,attrSet, 0 );
	}

	#ifdef _DEBUG

	#endif

	printForestInC( forestRoots );
	
	return 0;
}
