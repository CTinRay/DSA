
#include <iostream>
#include <map>
#include <vector>
//#include "tree_generator.hpp"
#include "test_util.hpp"
#include <algorithm>
typedef unsigned int uint;
typedef double Threshold;
typedef uint Attr;

double calculateConfusion( uint y, uint n ){
	double sum = y + n;
	return 1 - ((double) y / sum) * ((double) y / sum) - ((double) n / sum) * ((double) n / sum);
}


double calculateTotalConfusion( uint y1, uint n1, uint y2, uint n2 ){
	double sum = y1 + y2 + n1 + n2 ;
	double sum1 = y1 + n1;
	double sum2 = y2 + n2;
	return ( sum1 / sum ) * calculateConfusion( y1 , n1 ) + ( sum2 / sum ) * calculateConfusion( y2 , n2 ); 
}

	

#define YES 1
#define NO -1
#define NEGATIVE_INF -2
Node* resolveTree( std::map<Attr,std::vector<Example*> >&orderByAttr, double epsilon){
	//count Yes and No
	uint nYes = 0, nNo = 0;
	for( auto it = orderByAttr.begin() -> second.begin() ; 
	     it != orderByAttr.begin() -> second.end() ; ++ it ){
		if( (*it) -> decision == YES ){
			nYes += 1;
		}else{
			nNo += 1;
		}
	}
	if( calculateConfusion( nYes , nNo ) <= epsilon ){
		Node*leave = new Node;
		leave -> decision = nYes > nNo ? YES : NO;
		return leave;
	}
	//Find best threshold
	double minConfusion = 1;
	Attr bestAttr = -1;
	Threshold bestThreshold = NEGATIVE_INF;
	for( auto it = orderByAttr.begin() ; it != orderByAttr.end() ; ++it ){
		Attr attr = it -> first;
	        auto&order = it -> second;
		uint nYesLeft = 0, nNoLeft = 0;
		uint nYesRight = nYes, nNoRight = nNo;
		for( uint i = 0 ; i < order.size() - 1 ; ++i ){
		        if( order[i] -> decision == YES ){
				nYesRight -= 1;
				nYesLeft += 1;
			}else{
				nNoRight -= 1;
				nNoLeft += 1;
			}
			double confusion = calculateTotalConfusion( nYesRight, nNoRight, nYesLeft, nNoLeft );
			if( confusion < minConfusion ){
				minConfusion = confusion;
				bestAttr = attr;
				bestThreshold = ( order[i] -> attrs[attr] + order[i+1] -> attrs[attr] ) / 2;
			}
		}
	}
	
	std::map<Attr,std::vector<Example*> >orderByAttrLeft;
	std::map<Attr,std::vector<Example*> >orderByAttrRight;
	for( auto itMap = orderByAttr.begin(); itMap != orderByAttr.end() ; ++itMap ){
		Attr attr = itMap -> first;
		for( auto itVec = itMap -> second.begin();
		     itVec != itMap -> second.end();
		     ++itVec ){
			if( (*itVec) -> attrs[ bestAttr ] < bestThreshold ){
				orderByAttrLeft[ attr ].push_back( (*itVec));
			}else{
				orderByAttrRight[ attr ].push_back( (*itVec) );
			}
		}
	}
	Node*node = new Node;
	node -> threshold = bestThreshold;
	node -> attr = bestAttr;
	node -> left = resolveTree( orderByAttrLeft ,epsilon );
	node -> right = resolveTree( orderByAttrRight ,epsilon );
	return node;
}
	

Node* buildTree(std::vector< Example* >&examples, std::set<Attr>attrSet, double epsilon){
	std::map<Attr,std::vector<Example*> >orderByAttr;
	for( auto setIt = attrSet.begin() ; setIt != attrSet.end(); ++setIt ){
	        Attr attr = *setIt;
		std::vector< Example* >&order = orderByAttr[attr];
		order = examples;
		std::sort(order.begin(), order.end(), [attr](Example*a, Example*b) -> bool {
				return a -> attrs[ attr ] < b -> attrs[ attr ];
			} );	
		#ifdef _DEBUG
		std::cout << "Ordered by " << attr << std::endl;
		printVec( order );
		#endif 
	        orderByAttr[ attr ] = order;
	}
	return resolveTree( orderByAttr, epsilon );
}


