#include <iostream>
#include <string>
#include "binomial_heap.h"

struct Tuple{
	int priority;
	int taskID;
};

bool operator > ( Tuple&t1, Tuple&t2){
	if( t1.priority > t2.priority ){
		return true;
	}
	if( t1.taskID < t2.taskID ){
		return true;
	}
	return false;
}

int main(){
	int nComputer, minTask;
	std::string command;
	std::cin >> nComputer >> minTask ;
	BinomialHeap<Tuple>*taskQueues = new BinomialHeap<Tuple> [ nComputer + 1 ] ;
	while( std::cin >> command ){
		if( command == "assign" ){
			int computerID, taskID, priority;
			std::cin >> computerID >> taskID >> priority;
			Tuple tuple = { priority, taskID };
			taskQueues[ computerID ].insert( tuple );
			std::cout << "There are " << taskQueues[ computerID ].getSize() 
				  << " tasks on computer " << computerID << std::endl;
		}
		else if( command == "execute" ){
			int computerID;
			std::cin >> computerID;
			int topPriority = taskQueues[ computerID ].top().priority;
			while( taskQueues[ computerID ].top().priority == topPriority ){
				int taskID = taskQueues[ computerID ].top().taskID;
				std::cout << "Computer " << computerID 
					  << " execute task " << taskID << std::endl;
				taskQueues[ computerID ].pop();
				#ifdef _DEBUG
				std::cout << "top now is " << taskQueues[ computerID ].top().priority << std::endl;
				#endif				
			}
		}
		else if( command == "merge" ){
			int computerID1, computerID2;
			std::cin >> computerID1 >> computerID2;
			if( taskQueues[ computerID2 ].getSize() < minTask ){
				std::cout << "Merging request failed." << std::endl;
			}else{			
				taskQueues[ computerID1 ].merge( taskQueues[ computerID2 ] );
				std::cout << "The largest priority number is " 
					  << taskQueues[ computerID1 ].top().priority
					  << " on " << computerID1 << std::endl;
			}
		}
	}
}