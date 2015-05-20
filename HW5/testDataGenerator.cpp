#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "distributed_system.hpp"
#define nDataDefault 100
#define nComputerDefault 20
#define nMinTaskDefault 2
#define priorityBoundDefault 300



int getRand(int upperBound ){
	return rand() % upperBound;
}



#define nCmdType 3
enum cmds{ cmdAssign, cmdExecute, cmdMerge };

int main(int argc,char**argv){
	#ifdef _DEBUG
	std::cout << argc << std::endl;
	#endif
	int nData = nDataDefault;
	int nComputer = nComputerDefault;
	int nMinTask = nMinTaskDefault;
	int priorityBound = priorityBoundDefault;
	switch( argc ){
	case 5:
		priorityBound = atoi( argv[4] );
	case 4:
		nMinTask = atoi( argv[3] );
	case 3:
		nComputer = atoi( argv[2] );
	case 2:
		nData = atoi( argv[1] );
	}	
	std::vector< BinomialHeap<Tuple> >taskQueues(nComputer);
	std::fstream testIn("n" + std::to_string( nData ) + "c" + std::to_string( nComputer ) + 
			    "w" + std::to_string( nMinTask ) + ".in" , std::fstream::out);
	//std::fstream testOut("n" + nData + "c" + nComputer + "w" + nMinTask + ".ans" , std::fstream::out);
	testIn << nComputer << " " << nMinTask << std::endl;
	int taskCounter = 0;
	srand( time(NULL));	

	#ifdef _DEBUG
	for( int i = 0 ; i < 10000 ; ++i ){
		testIn << "assign " << 0 
		       << " " << taskCounter
		       << " " << i << std::endl;
		assign( 0, taskCounter++, i, taskQueues );
	}

	while( taskQueues[ 0 ].getSize() > 0 ){
		testIn << "execute " << 0 << std::endl;
		execute( 0, taskQueues );
	}
	#endif
		
	for( int i = 0 ; i < nData ; ++i ){
		int cmdType = getRand( 100 );
		int taskID = taskCounter ++;
		int priority = getRand( priorityBound );
		int computerID1 = getRand( nComputer ) ;
		int computerID2 = getRand( nComputer ) ;			

		if( cmdType <= 60 ){
			testIn << "assign " << computerID1 
			       << " " << taskID 
			       << " " << priority << std::endl;
			assign( computerID1, taskID, priority, taskQueues );
		}
		else if( cmdType <= 80 ){ 
			if( taskQueues[ computerID1 ].getSize() > 0 ){
				testIn << "execute " << computerID1 << std::endl;
				execute( computerID1, taskQueues );
			}
		}
		else {
			if( computerID1 != computerID2 ){
				testIn << "merge " << computerID1 << " " << computerID2 << std::endl;
				merge( computerID1, computerID2, nMinTask, taskQueues );
			}
		}
	}
	testIn.close();
}
	
	
