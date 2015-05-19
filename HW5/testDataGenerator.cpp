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
	int taskCounter = 0;
	srand( time(NULL));	
	
	for( int i = 0 ; i < 10 ; ++i ){
		testIn << "assign " << 0 
		       << " " << taskCounter
		       << " " << i << std::endl;
		assign( 0, taskCounter++, i, taskQueues );
	}

	for( int i = 0 ; i < 10 ; ++i ){
		if( taskQueues[ 0 ].getSize() > 0 ){
			testIn << "execute " << 0 << std::endl;
		}
		execute( 0, taskQueues );
	}
		
	for( int i = 0 ; i < nData ; ++i ){
		int cmdType = getRand( nCmdType );
		int taskID = taskCounter ++;
		int priority = getRand( priorityBound );
		int computerID1 = getRand( nComputer ) ;
		int computerID2 = getRand( nComputer ) ;			

		switch( cmdType ){
		case cmdAssign:
			testIn << "assign " << computerID1 
			       << " " << taskID 
			       << " " << priority << std::endl;
			assign( computerID1, taskID, priority, taskQueues );
			break;
		case cmdExecute:
			if( taskQueues[ computerID1 ].getSize() > 0 ){
				testIn << "execute " << computerID1 << std::endl;
			}
			execute( computerID1, taskQueues );
			break;
		case cmdMerge:
			testIn << "merge " << computerID1 << " " << computerID2 << std::endl;
			merge( computerID1, computerID2, nMinTask, taskQueues );
			break;
		}
	}
	testIn.close();
}
	
	
