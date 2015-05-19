//#include "binomial_heap.h"
#include "distributed_system.hpp"
#include <vector>
#include <iostream>

bool operator > ( Tuple&t1, Tuple&t2){
	if( t1.priority > t2.priority ){
		return true;
	}else if( t1.priority < t2.priority ){
		return false;
	}
	if( t1.taskID < t2.taskID ){
		return true;
	}
	return false;
}

void merge(int computerID1, int computerID2, int minTask, std::vector< BinomialHeap<Tuple> >&taskQueues ){
	if( taskQueues[ computerID2 ].getSize() < minTask ){
		std::cout << "Merging request failed." << std::endl;
	}else{			
		taskQueues[ computerID1 ].merge( taskQueues[ computerID2 ] );
		std::cout << "The largest priority number is now " 
			  << taskQueues[ computerID1 ].top().priority
			  << " on " << computerID1 << "." << std::endl;
	}
}       

void execute( int computerID, std::vector< BinomialHeap<Tuple> >&taskQueues){
	if( taskQueues[ computerID ].getSize() == 0 ){
		std::cout << "compute " << computerID << " not execute." << std::endl;
		return;
	}
	int topPriority = taskQueues[ computerID ].top().priority;
	while( taskQueues[ computerID ].getSize() > 0 &&
	       taskQueues[ computerID ].top().priority == topPriority ){
		int taskID = taskQueues[ computerID ].top().taskID;
		std::cout << "Computer " << computerID 
			  << " executed task " << taskID << "." << std::endl;
		#ifdef _DEBUG
		std::cout << "                    priority:" << taskQueues[ computerID ].top().priority << std::endl;
		#endif
		taskQueues[ computerID ].pop();
	}
}

void assign( int computerID, int taskID, int priority, std::vector< BinomialHeap<Tuple> >&taskQueues ){
	Tuple tuple = { priority, taskID };
	taskQueues[ computerID ].insert( tuple );
	std::cout << "There are " << taskQueues[ computerID ].getSize() 
		  << " tasks on computer " << computerID << "." << std::endl;
}
