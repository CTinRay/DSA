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
	
	//std::cout << "Start Reading" << std::endl;
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
				std::cout << "EOF" ;
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
	readFile( argv[1] , database );
	database -> sort();
	database -> updateCTR();
	std::string command;
	std::cin >> command;
	while( command != "quit" ){
		std::cout << "********************" << std::endl;
		if( command == "get" ){
			uint userID, adID, queryID, position, depth;
			std::cin >> userID >> adID >> queryID >> position >> depth;
			database -> printGet( userID , adID , queryID, position, depth );
		}
		else if( command == "clicked" ){
			uint userID;
			std::cin >> userID ;
			database -> printClicked( userID );			
		}
		else if( command == "impressed" ){
			uint userID1, userID2;
			std::cin >> userID1 >> userID2;
			database -> printImpressed( userID1 , userID2 );
		}
		else if( command == "profit" ){
			uint adID;
			double thita;
			std::cin >> adID >> thita ;
			database -> printProfit( adID , thita );
		}else{
			std::cout << "Not a valid command" << std::endl;
			break;
		}
		
		std::cout << "********************" << std::endl ;
		std::cin >> command;
	}
	return 0;

}
