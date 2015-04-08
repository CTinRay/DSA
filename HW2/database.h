#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <set>
typedef unsigned int uint; 
typedef unsigned short ushort;
typedef unsigned long long int ulli;
#define NUser 24000000



class Database{
public:
	struct ClickImpress{
		unsigned short click;
		unsigned int impress;
	};
	Database();
	~Database();
	void insert(uint click, uint impression , ulli displayURL , uint adID , uint advertiserID , ushort depth , ushort position ,
		    uint queryID, uint keyword, uint title, uint description, uint userID);
	void printGet( uint userID, uint adID , uint queryID , ushort position , ushort depth );
	void printClicked( uint userID  );
	void printImpressed( uint userID1 , uint userID2 );
	void printProfit( uint adID, double thita);
private:

	class AdProperties{
	public:
		AdProperties();
		void insert(ulli URL , ushort adverID , uint keyword , uint title , uint description);
		void printProperties();			
	private:

		struct Property{
			ulli URL;
			ushort adverID;
			uint keyword;
			uint title;
			uint description;
			bool operator == ( const Property p )const{
				if( URL == p.URL &&
				    adverID == p.adverID &&
				    keyword == p.keyword &&
				    title == p.title &&
				    description == p.description ){
					return true;
				}
				return false;
			}
		};

		struct propertyCmp{
			bool operator()(const Property&p1, const Property&p2)const{				
				if( p1 == p2 ){
					return false;
				}
				if( p1.URL < p2.URL ){
					return true;
				}else{
					return false;
				}
				if( p1.adverID < p2.adverID ){
					return true;
				}else{
					return false;
				}
				if( p1.keyword < p2.keyword ){
					return true;
				}else{
					return false;
				}
				if( p1.title < p2.title ){
					return true;
				}else{
					return false;
				}
				if( p1.description < p2.description ){
					return true;
				}else{
					return false;
				}
			}		
		};
		std::set<Property,propertyCmp> propertySet;		

	};


	class User{
		friend class Database;
	public:		
		User();		
		void addRecord( uint adID, uint queryID, ushort position , ushort depth , ushort click , uint impress );
		void print( uint adID, uint queryID, uint position, uint depth );
		void printClicked();
		void updateCTR();
		double getCTR(uint adID);
		class Database;
	private:

		struct CTR{
			uint click = 0;
			uint impress = 0;
			double CTR = 0;
		};
		class Query{
		public:
			Query();
			bool isClicked();
			void addRecord( ushort position , ushort depth , ushort click , uint impress );
			ClickImpress getRecord( ushort position , ushort depth );
		private:      
			bool clicked = 0;//true if it has at least been clicked once.
			ClickImpress record[3][3] = {{{0,0}}};
		};
		std::map< uint, std::map<uint,Query> > adMap;
		std::unordered_map< uint, CTR >adCTRMap;
		bool CTRUpToDate = 0;
	};




	User*userList;
	std::unordered_map<uint,AdProperties>adPropertyMap;

};

