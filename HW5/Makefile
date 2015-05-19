CPPFLAGS = -O2 -std=c++11 -Wall 
CPPFLAGS_DBG = -std=c++11 -Wall -g3 -D_DEBUG
system:distributed_system.cpp system.cpp binomial_heap.h
	g++ $(CPPFLAGS) system.cpp distributed_system.cpp -o system
dbg:distributed_system.cpp system.cpp binomial_heap.h
	g++ $(CPPFLAGS_DBG) system.cpp distributed_system.cpp -o system
testDataGenerator:distributed_system.cpp testDataGenerator.cpp binomial_heap.h
	g++ $(CPPFLAGS) distributed_system.cpp testDataGenerator.cpp -o testDataGenerator
testDataGenerator_dbg:distributed_system.cpp testDataGenerator.cpp binomial_heap.h
	g++ $(CPPFLAGS_DBG) distributed_system.cpp testDataGenerator.cpp -o testDataGenerator
