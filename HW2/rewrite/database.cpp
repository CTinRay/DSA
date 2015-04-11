#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "database.h"
#include <cstdlib>
#include <unordered_map>
#include <algorithm>

Database::Database(){

}

void Database::insert(ushort&click,
		      uint&impression ,
		      ulli&displayURL ,
		      uint&adID ,
		      uint&advertiserID ,
		      std::uint8_t&depth ,
		      std::uint8_t&position ,
		      uint&queryID,
		      uint&keywordID,
		      uint&titleID,
		      uint&descriptionID,
		      uint&userID){
	const std::set<AdProperty,AdPropertyCmp>::iterator itProperty = adPropertyMap[ adID ].insert( displayURL, userID, advertiserID, keywordID, titleID, descriptionID, click, impression );
	DataEntry&entry = dataEntries[ nEntry++ ];
	entry.userID=userID;
	entry.adID = adID;
	entry.queryID = queryID;
	entry.click = click;
	entry.impression = impression;
	entry.pairDepthPosition =( (depth << 2) + position);
	//std::cout << (uint) entry.pairDepthPosition << std::endl;
	entry.ptrProperty = &(*itProperty);
	//std::cout << nEntry << ":" << (int)depth << "," << (int)position << std::endl;
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

/*
bool operator<(const std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&a,
	       const std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator&b){
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
*/


int Database::getFirstMatchIndex(const DataEntry& entry ){
	int lowerBound = 0;
	int upperBound = nEntry;

	while( upperBound - lowerBound 	> 0 ){
		int mid = lowerBound + ((upperBound - lowerBound) >> 1);
		//		std::cout << "low:" << lowerBound << "\tmid:" << mid << "\tup:" << upperBound << std::endl;
		if( entryCmp( dataEntries[ mid ] , entry)  ){
			lowerBound = mid + 1;
		}else{
			upperBound = mid;
		}
	}
		
	return upperBound;
}
	

void Database::printGet( uint userID, uint adID , uint queryID , ushort position , ushort depth ){
	if( !ready ){
		///		std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry entry;
	entry.userID = userID;
	entry.adID = adID;
	entry.queryID = queryID;
	std::uint8_t pairDP = (depth << 2) + position;
	entry.pairDepthPosition = pairDP;
	int firstEntryIndex = getFirstMatchIndex( entry );
	int click = 0;
	int impression = 0;
       	///std::cout << (int) pairDP << std::endl;
	for( int i = firstEntryIndex ;
	     //	     std::cout << i << ":" << 
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
		//std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry targetEntry;
	targetEntry.userID = userID;
	int index = getFirstMatchIndex( targetEntry );
	//std::cout << index << std::endl;
	while( dataEntries[ index ].userID == userID ){
		auto adID = dataEntries[index].adID;
		auto queryID = dataEntries[index].queryID;
		if( dataEntries[ index ].click > 0 ){
			std::cout << adID << "\t"
				  << queryID << std::endl;
			while( dataEntries[index].userID == userID &&
			       dataEntries[index].adID == adID &&
			       dataEntries[index].queryID == queryID ){
				index += 1;
			}
			
		}else{
			index += 1;
		}
	}
}



void Database::printImpressed( uint userID1 , uint userID2 ){
	if( !ready ){
		//std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry targetEntry1;
	targetEntry1.userID = userID1;
	DataEntry targetEntry2;
	targetEntry2.userID = userID2;
	int index1 = getFirstMatchIndex( targetEntry1 );	
	int index2 = getFirstMatchIndex( targetEntry2 );
	std::cout << "index1:" << index1 << std::endl
		  << "index2:" << index2 << std::endl;
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
				if( dataEntries[index1].ptrProperty < dataEntries[index2].ptrProperty ){
					Database::printAdProperties( dataEntries[index1].ptrProperty );
					index1 += 1;
				}else if( dataEntries[index1].adID > dataEntries[index2].adID ){
					Database::printAdProperties( dataEntries[index2].ptrProperty );
					index2 += 1;
				}else if( dataEntries[index1].adID == dataEntries[index2].adID ){
					Database::printAdProperties( dataEntries[index1].ptrProperty );
					index1 += 1;
					index2 += 1;
				}
			}
		}
	}
}

void Database::printAdProperties( const AdProperty*ptrProperty){
	std::cout << "\t"
		  << ptrProperty -> displayURL << "\t"
		  << ptrProperty -> advertiserID << "\t"
		  << ptrProperty -> keywordID << "\t"
		  << ptrProperty -> titleID << "\t"
		  << ptrProperty -> descriptionID << std::endl;
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

int Database::entryCmp( const void*p1, const void*p2){
	DataEntry&a = *(DataEntry*)p1;
	DataEntry&b = *(DataEntry*)p2;
	if( ( a.userID < b.userID) ||
	    ( a.userID == b.userID && a.adID < b.adID ) ||
	    ( a.adID == b.adID && a.queryID < b.queryID ) ||
	    ( a.queryID == b.queryID &&
	      a.pairDepthPosition < b.pairDepthPosition  ) ||
	    ( a.pairDepthPosition == b.pairDepthPosition
	      && a.click < b.click ) ||
	    ( a.click == b.click &&
	      a.impression < b.impression )||
	    ( a.impression == b.impression && a.ptrProperty < b.ptrProperty )
	    ){
		return -1;
	    }
	    return 1;
};

bool Database::entryCmp( const DataEntry&a, const DataEntry&b){
	if( ( a.userID < b.userID) ||
	    ( a.userID == b.userID && a.adID < b.adID ) ||
	    ( a.adID == b.adID && a.queryID < b.queryID ) ||
	    ( a.queryID == b.queryID &&
	      a.pairDepthPosition < b.pairDepthPosition ) ||
	    ( a.pairDepthPosition == b.pairDepthPosition
	      && a.click < b.click ) ||
	    ( a.click == b.click &&
	      a.impression < b.impression )||
	    ( a.impression == b.impression && a.ptrProperty < b.ptrProperty )
	    ){
		return true;
	}
	return false;
};

void Database::sort(){
	std::cout << "start sorting" << std::endl;
	std::cout << "nEntry:" << nEntry << std::endl;
	//	std::sort( &(dataEntries[0]), &(dataEntries[nEntry-1]) , entryCmp );
	qsort(dataEntries,nEntry-1,sizeof(DataEntry), Database::entryCmp);
	std::cout << "end sorting" << std::endl;
	ready = true;
}

void Database::printList(){
	for( int i = 0 ; i < nEntry ; ++i ){
		std::cout << i << ":"
			  << dataEntries[i].userID << "\t"
			  << dataEntries[i].adID << "\t"
			  << dataEntries[i].queryID << "\t"
			  << (int) dataEntries[i].pairDepthPosition << "\t"
			  << dataEntries[i].click << "\t"
			  << dataEntries[i].impression << "\t"
			  << dataEntries[i].ptrProperty << std::endl;
	}
	//	std::cout << "CMP12:" << entryCmp( (void*) &dataEntries[1] , (void*) &dataEntries[2] ) << std::endl;
}
