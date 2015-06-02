#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <functional>
typedef unsigned int uint;
typedef double Threshold;
typedef uint Attr;


struct Node{
	Node*left;
	Node*right;
	Threshold threshold;
	Attr attr;
	short decision = 0;
};

struct Example{
	int decision;
	std::map<Attr,Threshold>attrs;
};


uint* newArrWithConsecutiveNumbers( int size);
double calculateConfusion( uint y, uint n );
double calculateTotalConfusion( uint y1, uint n1, uint y2, uint n2 );
void countYesAndNo( std::vector<Example*>*examples,uint&nYes,uint&nNo);
Node* resolveTree( std::map<Attr,std::vector<Example*> >&orderByAttr, double epsilon);	
Node* buildTree(std::vector< Example* >&examples, std::set<Attr>attrSet, double epsilon);
void printTreeInC( Node*node );
void printNodeInC( Node*node , std::string indent, std::function<void (int, std::string)> ret );
void printForestInC( std::vector<Node*>&roots );
