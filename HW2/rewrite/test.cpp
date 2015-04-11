#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
typedef unsigned long long int ulli;
typedef unsigned int uint;

struct AdProperty{
	ulli displayURL;
	uint advertiserID;
	uint keywordID;
	uint titleID;
	uint descriptionID;
};


struct AdPropertyCmp{
	bool operator()(const AdProperty&a,const AdProperty&b){
		if( ( a.displayURL < b.displayURL ) ||
		    ( a.displayURL == b.displayURL && a.advertiserID < b.advertiserID ) ||
		    ( a.advertiserID == b.advertiserID && a.keywordID < b.keywordID ) ||
		    ( a.keywordID == b.keywordID && a.descriptionID < b.descriptionID )
		    ){
			return true;
		}
		return false;
	}
};

int main(){		
	auto a = (std::set<AdProperty,AdPropertyCmp>::iterator) 0;
	auto b = (std::set<AdProperty,AdPropertyCmp>::iterator) 0;
	
	std::set<int>set1;
	std::set<int>::iterator it = set1.insert(100).first;
	void*vit =  (void*)&(*it);
	std::cout << (*(int*)vit) << std::endl;
	for( int i = 0 ; i < 1000000 ; ++i ){
		set1.insert( i );
	}
	std::cout << (*(int*)vit) << std::endl;						    
	
}

