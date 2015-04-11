#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "database.h"
#include <unordered_map>

Database::Database(){}

void Database::insert(ushort click, uint impression , ulli displayURL , uint adID , uint advertiserID , ushort depth , ushort position ,
		      uint queryID, uint keywordID, uint titleID, uint descriptionID, uint userID){
	auto itProperty = adPropertyMap[ adID ].insert( displayURL, userID, advertiserID, keywordID, titleID, descriptionID, click, impression );
	DataEntry&entry = dataEntries[ nEntry++ ];
	entry.userID=userID;
	entry.adID = adID;
	entry.queryID = queryID;
	entry.click = click;
	entry.impression = impression;
	entry.pairDepthPosition =(std::int8_t)(depth * 4 + position);
	entry.itProperty = itProperty;
};

std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator
Database::AdProperties::insert(ulli displayURL,
			       uint userID,uint advertiserID,
			       uint keywordID,
			       uint titleID,
			       uint descriptionID,
			       ushort click,
			       uint impression){
	
	CTR&ctr = userCTRMap[ userID ];
	ctr.click += click;
	ctr.impression += impression;
	return propertySet.insert( {displayURL,
				advertiserID,
				keywordID,
				titleID,
				descriptionID
				} ).first;

};

bool Database::entryCmp( const DataEntry&a, const DataEntry&b){
	Database::AdPropertyCmp cmp ;

	if( ( a.userID < b.userID) ||
	    ( a.userID == b.userID && a.adID < b.adID ) ||
	    ( a.adID == b.adID && a.queryID < b.queryID ) ||
	    ( a.queryID == b.queryID &&
	      a.pairDepthPosition < b.pairDepthPosition  ) ||
	    ( a.pairDepthPosition == b.pairDepthPosition
	      && a.impression < b.impression ) ||
	    ( a.impression == b.impression &&
	      a.click < b.click )||
	    ( a.click == b.click &&
	      b.itProperty == (std::set<AdProperty,AdPropertyCmp>::iterator) 0 ) ||
	    ( b.itProperty != (std::set<AdProperty,AdPropertyCmp>::iterator) 0 && cmp(*(a.itProperty),*(b.itProperty)) )
	    ){
		return true;
	    }
	    return false;
};

int Database::getFirstMatchIndex(const DataEntry& entry ){
	int lowerBound = 0;
	int upperBound = nEntry;

	while( upperBound - lowerBound 	> 0 ){
		int mid = lowerBound + ((upperBound - lowerBound) >> 1);
		if( entryCmp( dataEntries[ mid ] , entry) ){
			lowerBound = mid;
		}else{
			upperBound = mid;
		}
	}
		
	return upperBound;
}
	

void Database::printGet( uint userID, uint adID , uint queryID , ushort position , ushort depth ){
	if( !ready ){
		std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry entry;
	entry.userID = userID;
	entry.adID = adID;
	entry.queryID = queryID;
	int pairDP = depth*4 + position;
	entry.pairDepthPosition = pairDP;
	int firstEntryIndex = getFirstMatchIndex( entry );
	int click = 0;
	int impression = 0;
	for( int i = firstEntryIndex ;
	     dataEntries[i].userID == userID &&
		     dataEntries[i].adID == adID &&
		     dataEntries[i].queryID == queryID &&
		     dataEntries[i].pairDepthPosition == pairDP;
		     ++i
	     ){
		click += dataEntries[i].click;
		impression += dataEntries[i].impression;
	}
	std::cout << click << "\t" << impression << std::endl;
}

void Database::printClicked( uint userID  ){
	if( !ready ){
		std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry targetEntry;
	targetEntry.userID = userID;
	int firstMatchIndex = getFirstMatchIndex( targetEntry );
	for( int i = firstMatchIndex ; dataEntries[i].userID == userID ; ++i ){
		DataEntry&entry = dataEntries[i];
		std::cout << entry.adID << "\t"
			  << entry.queryID << std::endl;
		while( dataEntries[i].userID == userID &&
		       dataEntries[i].adID == entry.adID &&
		       dataEntries[i].queryID == entry.queryID ){
			i = i + 1;
		}
	}
}


bool operator<(std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&a,
	       std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&b){
	 if( a == b ){
		 return false;
	 }
	 Database::AdPropertyCmp cmp;
	 return cmp( *a , *b );
 };


bool operator>(std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&a,
	       std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&b){
	 if( a == b ){
		 return false;
	 }
	 Database::AdPropertyCmp cmp;
	 return cmp( *b , *a );
};

void Database::printImpressed( uint userID1 , uint userID2 ){
	if( !ready ){
		std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry targetEntry1;
	targetEntry1.userID = userID1;
	DataEntry targetEntry2;
	targetEntry2.userID = userID2;
	int index1 = getFirstMatchIndex( targetEntry1 );
	int index2 = getFirstMatchIndex( targetEntry2 );
	while( dataEntries[index1].userID == userID1 &&
	       dataEntries[index2].userID == userID2 ){
		if( dataEntries[index1].adID < dataEntries[index2].adID ){
			index1 += 1;
		}else if( dataEntries[index1].adID > dataEntries[index2].adID ){
			index2 += 1;
		}else if( dataEntries[index1].adID == dataEntries[index2].adID ){
			auto adID = dataEntries[index1].adID;
			std::cout << adID << std::endl;
			while( dataEntries[index1].userID == userID1 &&
			       dataEntries[index2].userID == userID2 &&
			       dataEntries[index1].adID == adID &&
			       dataEntries[index2].adID == adID ){
				if( dataEntries[index1].itProperty < dataEntries[index2].itProperty ){
					Database::printAdProperties( dataEntries[index1].itProperty );
					index1 += 1;
				}else if( dataEntries[index1].adID > dataEntries[index2].adID ){
					Database::printAdProperties( dataEntries[index2].itProperty );
					index2 += 1;
				}else if( dataEntries[index1].adID == dataEntries[index2].adID ){
					Database::printAdProperties( dataEntries[index1].itProperty );
					index1 += 1;
					index2 += 1;
				}
			}
		}
	}
}

void Database::printAdProperties(std::set<AdProperty,AdPropertyCmp>::iterator it){
	auto &adProperty = *it ;
	std::cout << "\t"
		  << adProperty.displayURL << "\t"
		  << adProperty.advertiserID << "\t"
		  << adProperty.keywordID << "\t"
		  << adProperty.titleID << "\t"
		  << adProperty.descriptionID << std::endl;
};

void Database::printProfit(uint adID, double thita){
	if( adPropertyMap.count( adID ) == 0 ){
		return;
	}
	auto&adProperties = adPropertyMap[adID];
	adProperties.printProfit( thita );
}

Database::AdProperties::AdProperties(){}

void Database::AdProperties::printProfit( double thita ){
	for( int uid = 0 ; uid < NUser ; ++uid ){
		if( userCTRMap.count( uid ) == 0 ){
			continue;
		}else{
			CTR&ctr = userCTRMap[uid];
			if( ctr.CTR == -1 ){
				ctr.CTR = (ctr.impression == 0 ) ? 0 : (double)ctr.click/(double)ctr.impression;
			}
			if( ctr.CTR >= thita ){
				std::cout << uid << std::endl;
			}
		}
	}
}

void Database::sort(){
	std::cout << "start sorting" << std::endl;
	std::sort( dataEntries, dataEntries + nEntry , entryCmp );
	std::cout << "end sorting" << std::endl;
	ready = true;
}
