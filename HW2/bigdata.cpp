#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include "database.h"
			
void readFile( char*fileName , Database*database ){	
	std::fstream bigDataFS;
	bigDataFS.open( fileName , std::fstream::in );
	if( !bigDataFS.is_open() ){
		std::cout << "Open file error!!" << std::endl;
		return;
	}
	std::cout << "Start Reading" << std::endl;
	uint click;
	uint impression;
	ulli displayURL;
	uint adID;
	uint advertiserID;
	ushort depth;
	ushort position;
	uint queryID;
	uint keyword;
	uint title;
	uint description;
	uint userID;
	int n = 0;
	int m = 0;
	while( bigDataFS.peek() != EOF ){
		bigDataFS >> click
			  >> impression
			  >> displayURL
			  >> adID
			  >> advertiserID
			  >> depth
			  >> position
			  >> queryID
			  >> keyword
			  >> title
			  >> description
			  >> userID;
		database -> insert( click, impression, displayURL, adID, advertiserID, depth, position, queryID,
				    keyword, title, description, userID);

		if( n == 10000 ){
			std::cout << "Read:"  << std::endl;
			n = 0;
			m += 1;
		}
		n++;

	}		
}	

	


int main( int argc , char**argv ){
	Database*database = new Database;
	int wait;
	std::cin >> wait;
	std::cout << "Open File" << argv[1] << std::endl;
	readFile( argv[1] , database );
	std::cout << "Read finished" << std::endl;
	std::cin >> wait;
	std::cout << wait;
	return 0;
}
