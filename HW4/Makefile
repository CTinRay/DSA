CPPFLAGS = -O2 -std=c++11 -Wall 
CPPFLAGS_DBG = -std=c++11 -Wall -g3 -D_DEBUG
tree: tree.cpp tree_generator.hpp tree_generator.cpp
	g++ $(CPPFLAGS) tree.cpp tree_generator.cpp -o tree
forest: forest.cpp tree_generator.hpp tree_generator.cpp
	g++ $(CPPFLAGS) forest.cpp tree_generator.cpp -o forest
