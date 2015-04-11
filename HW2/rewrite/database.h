#include <iostream>
#include <map>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
typedef unsigned int uint; 
typedef unsigned short ushort;
typedef unsigned long long int ulli;
#define NUser 24000000
#define NEntries 150000000


class Database{
public:
	Database();
	~Database();
	void insert(ushort click, uint impression , ulli displayURL , uint adID , uint advertiserID , ushort depth , ushort position ,
		    uint queryID, uint keyword, uint title, uint description, uint userID);
	void printGet( uint userID, uint adID , uint queryID , ushort position , ushort depth );
	void printClicked( uint userID  );
	void printImpressed( uint userID1 , uint userID2 );
	void printProfit( uint adID, double thita);
	void sort();
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

private:			

	class AdProperties{
	public:
		AdProperties();
		std::set<AdProperty,AdPropertyCmp>::iterator
		insert(
		       ulli displayURL,
		       uint adID,uint advertiserID,
		       uint keywordID,
		       uint titleID,
		       uint descriptionID,
		       ushort click,
		       uint impression
		       );
		void printProfit( double thita );
			
	private:
		struct CTR{
			uint click = 0;
			uint impression = 0;
			double CTR = -1;
		};						    
		std::unordered_map<uint,CTR> userCTRMap;
		std::set<AdProperty,AdPropertyCmp>propertySet;
	};	


	struct DataEntry{
		uint userID = 0;
		uint adID = 0;
		uint queryID = 0;
		ushort click = 0;
		uint impression = 0;
		std::int8_t pairDepthPosition = 0;
		std::set<AdProperty,AdPropertyCmp>::iterator itProperty = (std::set<AdProperty,AdPropertyCmp>::iterator) 0;
	};


	friend AdProperty;
	std::map< uint , AdProperties > adPropertyMap;
	DataEntry dataEntries[ NEntries ];
	int nEntry = 0;
	bool ready = false;
	static bool entryCmp( const DataEntry&a, const DataEntry&b);
	int getFirstMatchIndex(uint userID, uint adID, uint queryID,uint depth, uint position);	
	int getFirstMatchIndex( const DataEntry&entry );
	static void printAdProperties(std::set<AdProperty,AdPropertyCmp>::iterator it);
	
	
};

