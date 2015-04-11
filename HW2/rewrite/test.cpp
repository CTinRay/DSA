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
	
	std::cout << ( a != (std::set<AdProperty,AdPropertyCmp>::iterator) 0 )<< std::endl;
	std::cout << sizeof(std::set<int>::iterator) << std::endl;
}

