#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "distributed_system.hpp"

BinomialHeap<Tuple>taskQueues[1000001];
int main(){
	int nComputer, minTask;
	std::string command;
	std::cin >> nComputer >> minTask ;
	//std::vector< BinomialHeap<Tuple> >taskQueues(nComputer);
	#ifdef _DEBUG
	int counter = 0 ;
	#endif
	while( std::cin >> command ){
		#ifdef _DEBUG
		counter += 1;
		#endif
		if( command == "assign" ){
			int computerID, taskID, priority;
			std::cin >> computerID >> taskID >> priority;
			assign( computerID, taskID, priority, taskQueues );
		}
		else if( command == "execute" ){
			int computerID;
			std::cin >> computerID;
			execute( computerID, taskQueues );
		}
		else if( command == "merge" ){
			int computerID1, computerID2;
			std::cin >> computerID1 >> computerID2;
			merge( computerID1, computerID2, minTask, taskQueues );
		}
	}
	#ifdef _DEBUG
	std::cout << "line: " << counter << std::endl;
	#endif
}
