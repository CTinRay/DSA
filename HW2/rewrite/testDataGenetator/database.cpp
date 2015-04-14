#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "database.h"
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <time.h>
#include <stdlib.h>

Database::Database(){
	//adPropertyPoolPtr = adPropertyPool;
	srand (time(NULL));
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
	DataEntry*entryPtr = &dataEntries[ nEntry ];
	entryPtrs[ nEntry++ ] = entryPtr;
	entryPtr->userID=userID;
	entryPtr->adID = adID;
	entryPtr->queryID = queryID;
	entryPtr->click = click;
	entryPtr->impression = impression;
	entryPtr->pairDepthPosition =( (depth << 2) + position);       
	entryPtr -> ptrProperty = &(*adPropertyMap[adID].insert( displayURL,
								 userID, advertiserID,
								 keywordID,
								 titleID,
								 descriptionID,
								 click,
								 impression ));
	//std::cout << (uint) entry.pairDepthPosition << std::endl;
	//std::cout << nEntry << ":" << (int)depth << "," << (int)position << std::endl;       

	/*
	adPropertyPoolPtr -> displayURL = displayURL;
	adPropertyPoolPtr -> advertiserID = advertiserID;
	adPropertyPoolPtr -> keywordID = keywordID;
	adPropertyPoolPtr -> titleID = titleID;
	adPropertyPoolPtr -> descriptionID = descriptionID;
	entryPtr -> ptrProperty = adPropertyMap[ adID ].insert( adPropertyPoolPtr );
	if( entryPtr -> ptrProperty == adPropertyPoolPtr ){
		adPropertyPoolPtr += 1;
		}*/	              
	
};

std::set<Database::AdProperty,Database::AdPropertyCmp>::iterator
Database::AdProperties::insert(ulli displayURL,
			       uint userID,uint advertiserID,
			       uint keywordID,
			       uint titleID,
			       uint descriptionID,
			       ushort click,
			       uint impression){
	
	//CTR&ctr = userCTRMap[ userID ];
	//ctr.click += click;
	//ctr.impression += impression;
	return propertySet.insert( {displayURL,
				advertiserID,
				keywordID,
				titleID,
				descriptionID
				} ).first;
	
				};
/*
Database::AdProperty*Database::AdProperties::insert( Database::AdProperty*adPropertyPtr ){
	auto pair = propertyPtrSet.insert( adPropertyPtr );
	if( pair.second ){ return adPropertyPtr; };
	return *(pair.first);
}
*/
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
		if( entryCmpA( dataEntries[ mid ] , entry)   ){
			lowerBound = mid + 1;
		}else{
			upperBound = mid;
		}
	}
		
	return upperBound;
}
	

int Database::getFirstMatchPtrIndex(const DataEntry* entry,int lowerBound = 0  ){
	int upperBound = nEntry;
	while( upperBound - lowerBound 	> 0 ){
		int mid = lowerBound + ((upperBound - lowerBound) >> 1);
		//std::cout << "low:" << lowerBound << "\tmid:" << mid << "\tup:" << upperBound << std::endl;
		if( entryPtrCmpA( entryPtrs[ mid ] , entry)   ){
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
	int firstEntryIndex = getFirstMatchPtrIndex( &entry );
	int click = 0;
	int impression = 0;
       	///std::cout << (int) pairDP << std::endl;
	for( int i = firstEntryIndex ;
	     entryPtrs[i] -> userID == userID &&
		     entryPtrs[i] -> adID == adID &&
		     entryPtrs[i] -> queryID == queryID &&
		     entryPtrs[i] -> pairDepthPosition == pairDP;
	     ++i
	     ){
		click += entryPtrs[i] -> click;
		impression += entryPtrs[i] -> impression;
	}
	std::cout << click << " " << impression << std::endl;
}

void Database::printClicked( uint userID  ){
	if( !ready ){
		//std::sort( dataEntries, dataEntries + nEntry , entryCmp );
		ready = true;
	}
	DataEntry targetEntry;
	targetEntry.userID = userID;
	int index = getFirstMatchPtrIndex( &targetEntry );
	//std::cout << index << std::endl;
	while( entryPtrs[index] -> userID == userID ){
		auto adID = entryPtrs[index] -> adID;
		auto queryID = entryPtrs[index] -> queryID;
		if( entryPtrs[ index ] -> click > 0 ){
			std::cout << adID << " "
				  << queryID << std::endl;
			while( entryPtrs[index] -> userID == userID &&
			       entryPtrs[index] -> adID == adID &&
			       entryPtrs[index] -> queryID == queryID ){
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
	int index1 = getFirstMatchPtrIndex( &targetEntry1 );	
	int index2 = getFirstMatchPtrIndex( &targetEntry2 );
	//Debug Info
	//std::cout << "index1:" << index1 << std::endl;
	//printEntry( index1 );
	//std::cout << "index2:" << index2 << std::endl;
	//printEntry( index2 );
	//End Info
	while( entryPtrs[index1] -> userID == userID1 &&
	       entryPtrs[index2] -> userID == userID2 ){

		if( entryPtrs[index1] -> adID < entryPtrs[index2] -> adID ){
			index1 += 1;
		}else if( entryPtrs[index1] -> adID > entryPtrs[index2] -> adID ){
			index2 += 1;
		}else if( entryPtrs[index1] -> adID == entryPtrs[index2] -> adID ){
			auto adID = entryPtrs[index1] -> adID;
			std::cout << adID << std::endl;
			std::set<AdProperty,AdPropertyCmp>uniq;
			while( entryPtrs[index1] -> userID == userID1 && entryPtrs[index1] -> adID == adID ){
				uniq.insert( *(entryPtrs[index1] -> ptrProperty) );
				++index1;
			}
			while( entryPtrs[index2] -> userID == userID2 && entryPtrs[index2] -> adID == adID ){
				uniq.insert( *(entryPtrs[index2] -> ptrProperty) );
				++index2;
			}
			for( auto it = uniq.begin() ; it != uniq.end() ; ++it ){
				printAdProperties( &(*it) );
			}
		}
	}
}

void Database::printAdProperties( const AdProperty*ptrProperty){
	std::cout << "\t"
		  << ptrProperty -> displayURL << " "
		  << ptrProperty -> advertiserID << " "
		  << ptrProperty -> keywordID << " "
		  << ptrProperty -> titleID << " "
		  << ptrProperty -> descriptionID << std::endl;
};

void Database::printProfit(uint adID, double thita){
	if( adPropertyMap.count( adID ) == 0 ){
		return;
	}
	int lowerBound = 0;
	for( uint userID = 0 ; userID < NUser && lowerBound < nEntry - 1  ; ++userID ){
		DataEntry entry;
		entry.userID = userID;
		entry.adID = adID;		
		//std::cout << "lower bound:" << lowerBound << std::endl;
		int index = getFirstMatchPtrIndex( &entry );// , lowerBound );
		//std::cout << "firstMastch" << index << std::endl;
		int impression = 0;
		int clicked = 0;		
		while( index < nEntry - 1&&
		       entryPtrs[index] -> userID == userID &&
		       entryPtrs[index] -> adID == adID 
		       ){
			impression += entryPtrs[index] -> impression;
			clicked += entryPtrs[index] -> click;
			index += 1;
			//std::cout << index << std::endl;
		}
		//std::cout << index << std::endl;
		if( impression > 0 && (double)clicked/(double)impression >= thita ){
			std::cout << userID << std::endl;
		}
		lowerBound = index;
	}

}

Database::AdProperties::AdProperties(){}

void Database::AdProperties::printProfit( double thita ){

	for( int uid = 0 ; uid < NUser ; ++uid ){
		if( userCTRMap.count( uid ) == 0 ){
			continue;
		}else{
			CTR&ctr = userCTRMap[uid];
			if( ctr.CTR >= thita ){
				std::cout << uid << std::endl;
			}
		}
	}
}

int Database::entryCmp( const void*p1, const void*p2){
	DataEntry&a = *(DataEntry*)p1;
	DataEntry&b = *(DataEntry*)p2;
	if( entryCmpA( a , b ) ){
		return -1;
	}else{
		return 1;
	}		       
};

bool Database::entryPtrCmpA( const DataEntry*a , const DataEntry*b ){
	return entryCmpA( *a, *b );
}

bool Database::entryCmpA( const DataEntry&a, const DataEntry&b){
	if( a.userID < b.userID ) return true;	
	if( a.userID > b.userID ) return false;	
	if( a.adID < b.adID ) return true;
	if( a.adID > b.adID ) return false;
	if( a.queryID < b.queryID ) return true;
	if( a.queryID > b.queryID ) return false;
	if( a.pairDepthPosition < b.pairDepthPosition ) return true;
	if( a.pairDepthPosition > b.pairDepthPosition ) return false;
	//if( a.click < b.click ) return true;
	//if( a.click > b.click ) return false;
	//if( a.impression < b.impression ) return true;
	//if( a.impression > b.impression ) return false;
	//if( a.ptrProperty < b.ptrProperty ) return true;
	//if( a.ptrProperty > b.ptrProperty ) return false;
	return false;
};


bool Database::entryEqual( const DataEntry&a, const DataEntry&b){
	if( a.userID != b.userID ) return false;	
	if( a.adID != b.adID ) return false;
	if( a.queryID != b.queryID ) return false;
	if( a.pairDepthPosition != b.pairDepthPosition ) return false;
	//if( a.click != b.click ) return false;
	//if( a.impression != b.impression ) return false;
	//if( a.ptrProperty != b.ptrProperty ) return false;
	return true;
};



void Database::sort(){
	std::cout << "start sorting" << std::endl;
	std::cout << "nEntry:" << nEntry << std::endl;
	std::sort( entryPtrs , entryPtrs + nEntry - 1 , entryPtrCmpA );
	//printList();
	//std::sort( dataEntries, dataEntries + nEntry - 1 , entryCmpA );
	//qsort(dataEntries,nEntry-1,sizeof(DataEntry), Database::entryCmp);
	std::cout << "end sorting" << std::endl;
	ready = true;
}

void Database::printList(){
	for( int i = 0 ; i < nEntry ; ++i ){
		std::cout << i << ":"
			  << entryPtrs[i] -> userID << "\t"
			  << entryPtrs[i] -> adID << "\t"
			  << entryPtrs[i] -> queryID << "\t"
			  << (int) entryPtrs[i] -> pairDepthPosition << "\t"
			  << entryPtrs[i] -> click << "\t"
			  << entryPtrs[i] -> impression << "\t"
			  << entryPtrs[i] -> ptrProperty << std::endl;
	}
	//	std::cout << "CMP12:" << entryCmp( (void*) &dataEntries[1] , (void*) &dataEntries[2] ) << std::endl;
	checkSort();
}

void Database::checkSort(){
		for( int i = 0 ; i < nEntry -1 ; ++i ){
		if( !entryCmpA(dataEntries[i],dataEntries[i+1])
		    && !entryEqual( dataEntries[i] , dataEntries[i+1]) ){
			/*std::cout << "Sort Err at :  " << i << std::endl;
			std::cout << "nEntry" << nEntry << std::endl;
			return;*/
		}
		}
	std::cout << "checkSort End!!" << std::endl;
}
	
	
			
void Database::printEntry( int i ){
	std::cout << entryPtrs[i] -> userID << "\t"
			  << entryPtrs[i] -> adID << "\t"
			  << entryPtrs[i] -> queryID << "\t"
			  << (int) entryPtrs[i] -> pairDepthPosition << "\t"
			  << entryPtrs[i] -> click << "\t"
			  << entryPtrs[i] -> impression << "\t"
			  << entryPtrs[i] -> ptrProperty << std::endl;
}


	

	
void Database::genImpress( int n ){
	std::set<uint>SET1;
	std::set<uint>SET2;
	while( n > 0 ){
		int randNum1 = entryPtrs[ rand() % (nEntry - 1) ] -> userID;
		int randNum2 = entryPtrs[ rand() % (nEntry - 1) ] -> userID;
		if( SET1.count( randNum1 ) == 0 ){
			SET1.insert( randNum1 );				
			n -= 1;
		std::cout << "impressed "
			  << randNum1  << " "
			  << randNum2  << std::endl;

		}
	}



	for( int i = 0 ; i < n ; ++i ){
		std::cout << "impressed "
			  << entryPtrs[ rand() % (nEntry - 1) ] -> userID << " "
			  << entryPtrs[ rand() % (nEntry - 1) ] -> userID << std::endl;
	}
}

void Database::genClicked( int n ){
	std::unordered_set< int > SET;
	while( n > 0 ){

		int randNum = entryPtrs[ rand() % (nEntry - 1) ] -> userID;
		if( SET.count( randNum ) == 0 ){
			SET.insert( randNum );				
			std::cout << "clicked " << randNum << std::endl;
			n -= 1;
		}
	}
}

void Database::genGet( int n ){
	for( int i = 0 ; i < n ; ++i ){
		int randInd = rand() % (nEntry - 1);
		std::cout << "get "
			  << entryPtrs[randInd] -> userID << " "
			  << entryPtrs[randInd] -> adID << " "
			  << entryPtrs[randInd] -> queryID << " "
			  << (entryPtrs[randInd] -> pairDepthPosition >> 2) << " "
			  << (entryPtrs[randInd] -> adID & 3)<< std::endl;
	}
}


void Database::genProfit( int n ){
	std::set<int>SET;
	while( n > 0 ){
		int randNum = entryPtrs[ rand() % (nEntry - 1) ] -> adID;
		if( SET.count( randNum ) == 0 ){
			SET.insert( randNum );				
			std::cout << "profit "
				  << randNum << " "
				  << (double)(rand()%1000)/1000.0
				  << std::endl;
			n -= 1;
		}
	}
}
