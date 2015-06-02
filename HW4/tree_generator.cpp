
#include <iostream>
#include <map>
#include <vector>
//#include "tree_generator.hpp"
#include "test_util.hpp"
#include <algorithm>
#include <functional>
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

Node*makeLeave(int nYes, int nNo){
	Node*leave = new Node;
	leave -> decision = nYes > nNo ? YES : NO;
	return leave;	
}

#define NEGATIVE_INF -2
Node* resolveTree( std::map<Attr,std::vector<Example*> >&orderByAttr, double epsilon){
	#ifdef _DEBUG
	static int counter = 0;
	counter ++ ;
	#endif
	//count Yes and No
	uint nYes = 0, nNo = 0;
	// for( auto it = orderByAttr.begin() -> second.begin() ; 
	//      it != orderByAttr.begin() -> second.end() ; ++ it ){
	// 	if( (*it) -> decision == YES ){
	// 		nYes += 1;
	// 	}else{
	// 		nNo += 1;
	// 	}
	// }
	for( uint i = 0 ; i < orderByAttr.begin() -> second.size() ; ++ i ){
		if( orderByAttr.begin() -> second[i] -> decision == YES ){
			nYes += 1;
		}else{
			nNo += 1;
		}
	}
	//check epsilon
	if( calculateConfusion( nYes , nNo ) <= epsilon ){
		return makeLeave( nYes , nNo );
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
			if( confusion < minConfusion && order[i] -> attrs[attr] != order[i+1] -> attrs[attr] ){
				minConfusion = confusion;
				bestAttr = attr;
				bestThreshold = ( order[i] -> attrs[attr] + order[i+1] -> attrs[attr] ) / 2;
			}
		}
	}
	
	if( bestThreshold == NEGATIVE_INF ){
		return makeLeave( nYes, nNo );
	}
	
	auto & orderByAttrLeft = *(new std::map<Attr,std::vector<Example*> >) ;
        auto & orderByAttrRight = *(new std::map<Attr,std::vector<Example*> >);
	for( auto itMap = orderByAttr.begin(); itMap != orderByAttr.end() ; ++itMap ){
		Attr attr = itMap -> first;
		std::vector<Example*>&vecExamplePtrs = itMap -> second;
#ifdef _DEBUG
		// std::cout << "order by " << attr << " " << counter << std::endl;
		// printVec( vecExamplePtrs ); 
#endif
		for( uint i = 0 ; i < vecExamplePtrs.size() ; ++ i  ){
			// if( vecExamplePtrs[i] -> attrs.count( bestAttr ) == 0 ){
			// 	continue;
			// }
			if( vecExamplePtrs[i] -> attrs[ bestAttr ] <= bestThreshold ){
				orderByAttrLeft[ attr ].push_back( vecExamplePtrs[i] );
			}else{
				orderByAttrRight[ attr ].push_back( vecExamplePtrs[i] );
			}
		}
	}
	#ifdef _DEBUG
	std::cout << "nY + nN " << nYes + nNo << std::endl;
	#endif
	Node*node = new Node;
	node -> threshold = bestThreshold;
	node -> attr = bestAttr;
	delete &orderByAttr;
	node -> left = resolveTree( orderByAttrLeft ,epsilon );
	//delete &orderByAttrLeft;
	node -> right = resolveTree( orderByAttrRight ,epsilon );
	//delete &orderByAttrRight;	
	return node;
}
	

Node* buildTree(std::vector< Example* >&examples, std::set<Attr>attrSet, double epsilon){
	std::map<Attr,std::vector<Example*> >&orderByAttr = *(new std::map<Attr,std::vector<Example*> >) ;
	for( auto setIt = attrSet.begin() ; setIt != attrSet.end(); ++setIt ){
	        Attr attr = *setIt;
		std::vector< Example* >&order = orderByAttr[attr];
		order = examples;
		std::sort(order.begin(), order.end(), [attr](Example*a, Example*b) -> bool {
				return a -> attrs[ attr ] < b -> attrs[ attr ];
			} );	
		#ifdef _DEBUG
		//std::cout << "Ordered by " << attr << std::endl;
		//printVec( order );
		#endif 
	        orderByAttr[ attr ] = order;
	}
	Node*result = resolveTree( orderByAttr, epsilon );
	//delete &orderByAttr;
	return result;
}



#define INDENT "   "
void printTreeInC( Node*root ){	
	std::cout << "int tree_predict(double *attr){" << std::endl;
	printNodeInC( root , INDENT, [](int decision, std::string indent){
			std::cout << indent << "return " << decision << ";" << std::endl; }
		);
	std::cout << "}" << std::endl;
}

void printForestInC( std::vector<Node*>&roots ){
	std::cout << "int forest_predict(double *attr){" << std::endl
		  << INDENT << "int vote = 0;" << std::endl;
	
	for( uint i = 0 ; i < roots.size() ; ++i ){
		std::cout << "tree" << i << "_predict:" << std::endl;
		printNodeInC( roots[i] , INDENT, [](int decision, std::string indent){
				std::cout << indent << "vote +=" << decision << ";" << std::endl; }
			);			
	}
	std::cout << "voting:" << std::endl;
	std::cout << INDENT << "return vote > 0 ? 1 : -1;" << std::endl
		  << "}" << std::endl;
	
}

void printNodeInC( Node*node , std::string indent, std::function<void (int, std::string)> ret ){
	if( node -> decision == 0 ){		
		std::cout << indent << "if( attr[ " << node -> attr << " ] <= " << node -> threshold << "){" << std::endl;
		printNodeInC( node -> left , indent + INDENT , ret);
		std::cout << indent << "}else{" << std::endl;
		printNodeInC( node -> right , indent + INDENT , ret);
		std::cout << indent << "}" << std::endl;
	}else{
		ret( node -> decision, indent );
		//std::cout << indent << "return " << node -> decision << ";" << std::endl;
	}
}

