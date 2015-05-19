#include "binomial_heap.h"
#include <vector>
#include <iostream>

struct Tuple{
	int priority;
	int taskID;
};

bool operator > ( Tuple&t1, Tuple&t2);
void merge(int computerID1, int computerID2, int minTask, std::vector< BinomialHeap<Tuple> >&taskQueues );
void execute( int computerID, std::vector< BinomialHeap<Tuple> >&taskQueues);
void assign( int computerID, int taskID, int priority, std::vector< BinomialHeap<Tuple> >&taskQueues );
