#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include "database.h"
#include <unordered_map>

Database::Database(){
	//	userList = new User[NUser];
	std::cout << "sizeof user:" << sizeof( User ) << std::endl;
	std::cout << "sizeof u_map:" << sizeof( std::unordered_map< uint ,uint > ) << std::endl;
	
	adPropertyMap.rehash( 641710 );

}

Database::~Database(){
	delete [] userList;
}

//Insert record to database
void Database::insert(uint click, uint impress , ulli displayURL , uint adID , uint advertiserID , ushort depth , ushort position ,
		      uint queryID, uint keyword, uint title, uint description, uint userID){
	//userList[ userID ].addRecord( adID, queryID, position , depth , click , impress );
	//adPropertyMap[ adID ].insert( displayURL , advertiserID , keyword , title , description);
	
}

//Implement the get function described in spec
void Database::printGet( uint userID, uint adID , uint queryID , ushort position , ushort depth ){
	userList[ userID ].print( adID, queryID, position, depth );
}

//Implement the clicked function described in spec
void Database::printClicked( uint userID  ){
	userList[ userID ].printClicked( );
}

//Implement the impressed function described in spec
void Database::printImpressed( uint userID1 , uint userID2 ){
	User&user1 = userList[ userID1 ];
	User&user2 = userList[ userID2 ];	       
	auto adIDIt1 = user1.adMap.begin();
	auto adIDIt2 = user2.adMap.begin();
	for(; adIDIt1 != user1.adMap.end() ; ++adIDIt1 ){
		while( adIDIt2 -> first <= adIDIt1 -> first ){
			if( adIDIt2 -> first == adIDIt1 -> first ){
				std::cout << ( adIDIt1 -> first ) << std::endl;
				adPropertyMap[ adIDIt1 -> first ].printProperties();
				break;
			}
			if( adIDIt2 == user2.adMap.end() ){
				break;
			}
			++adIDIt2;
		}
	}

}

void Database::printProfit(uint adID, double thita){
	for( int i = 0 ; i < NUser ; ++i ){
		if( userList[ i ].getCTR(adID) >= thita ){
			std::cout << i << std::endl;
		}
	}
}

//Constrct function of Query
Database::User::Query::Query(){
}

//Check if the query is clicked
bool Database::User::Query::isClicked(){
	return clicked;
}

//New query record
void Database::User::Query::addRecord( ushort position , ushort depth , ushort click , uint impress ){
	clicked = clicked == 1 || click == 1;
	record[position - 1][depth - 1].click += click;
	record[position - 1][depth - 1].impress += impress;
}

//get query record
Database::ClickImpress Database::User::Query::getRecord( ushort position , ushort depth ){
	return record[ position - 1 ][ depth - 1 ];
}

//Constructor of User
Database::User::User(){
}

void Database::User::updateCTR(){
	for( auto CTRit = adCTRMap.begin() ; CTRit != adCTRMap.end() ; ++CTRit ){
		CTR&ctr = CTRit -> second;
		ctr.CTR = ctr.impress == 0 ? 0 : (double)ctr.click / (double)ctr.impress ;
		/*		std::cout << "Ad:" << CTRit -> first
			  << "\tImpress: " << ctr.impress
			  << "\tClick:" << ctr.click
			  << "\tCTR:" << ctr.CTR 
			  << std::endl;*/
	}
}

double Database::User::getCTR(uint adID){
	if( adCTRMap.count( adID ) == 0 ){
		return -1;
	}
	if( !CTRUpToDate ){
		updateCTR();
	}
	return adCTRMap[adID].CTR;
}
	

void Database::User::addRecord( uint adID, uint queryID, ushort position , ushort depth , ushort click , uint impress ){
	adMap[ adID ][ queryID ].addRecord( position , depth , click , impress );
	CTR&ctr = (adCTRMap[ adID ]);
	ctr.impress += impress;
	ctr.click += click;	
	//	std::cout << "Impress: " << ctr.impress << "\tClick:" << ctr.click << std::endl;
	
}

//Build for get function
void Database::User::print( uint adID, uint queryID, uint position, uint depth ){
	std::cout << adMap[ adID ][ queryID ].getRecord( position , depth ).click << "\t"
		  << adMap[ adID ][ queryID ].getRecord( position , depth ).impress << std::endl;
}

//Build for click function
void Database::User::printClicked(){
	for( std::map<uint, std::map<uint,Query> >::iterator adIt = adMap.begin(); adIt != adMap.end() ; ++adIt ){
		uint adID = adIt -> first;
		std::map< uint , Query >&queryMap = adIt -> second;
		for( std::map< uint , Query > :: iterator queryIt = queryMap.begin() ;
		     queryIt != queryMap.end() ; ++queryIt ){
			uint queryID = queryIt -> first;
			Query&query = queryIt -> second;
			if( query.isClicked() ){
				std::cout << adID << "\t"
					  << queryID << std::endl;
			}
		}
	}
}

//Constructor of AdProperSet
Database::AdProperties::AdProperties(){

}


void Database::AdProperties::insert(ulli URL , ushort adverID , uint keyword , uint title , uint description){
	Property property = {
		URL,
		adverID,
		keyword,
		title,
		description
	};
	propertySet.insert(property);
}

						 

//Print the properties of the ad with the id
void Database::AdProperties::printProperties(){
	for( auto it = propertySet.begin(); it != propertySet.end(); ++ it ){
		std::cout << "\t"
			  << it -> URL << "\t"
			  << it -> adverID << "\t"
			  << it -> keyword << "\t"
			  << it -> title << "\t"
			  << it -> description << "\t"
			  << std::endl;
	}
}
