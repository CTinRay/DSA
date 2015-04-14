#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include "database.h"
			
void readFile( char*fileName , Database*database ){	
	std::fstream bigDataFS;
	std::fstream::sync_with_stdio(false);
	bigDataFS.open( fileName , std::fstream::in );
	if( !bigDataFS.is_open() ){
		std::cout << "Open file error!!" << std::endl;
		return;
	}
	std::cout << "Start Reading" << std::endl;
	ushort click;
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
	int n = 1;
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
		if( n == 1000000 ){
			//			std::cout << "Read:" << m*10000 << "\t"				
				/*				  << click << "\t" 
				  << impression << "\t"
				  << displayURL << "\t"
				  << adID << "\t"
				  << userID << "\t"*/
			//	  << std::endl;
			if( bigDataFS.eof() ){
				//std::cout << "EOF" ;
				break;
			}
			n = 0;
			m += 1;
		}
		n++;

		database -> insert( click, impression, displayURL, adID, advertiserID, (std::uint8_t&)depth, (std::uint8_t&)position, queryID,
				    keyword, title, description, userID);


	}		
}	

	


int main( int argc , char**argv ){
	Database*database = new Database();
	std::cout << "Open File" << argv[1] << std::endl;
	readFile( argv[1] , database );
	std::cout << "Read finished" << std::endl;
	database -> sort();
	//database -> printList();


	database -> checkSort();
	std::cout << "start" << std:: endl;
	database -> genProfit( 1200 );
	database -> genGet( 3600 );
	database -> genClicked( 2400 );
	database -> genImpress( 1200 );
	return 0;

}
