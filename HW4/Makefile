CPPFLAGS = -O2 -std=c++11 -Wall 
CPPFLAGS_DBG = -std=c++11 -Wall -g3 -D_DEBUG
tree: tree.cpp tree_generator.hpp tree_generator.cpp
	g++ $(CPPFLAGS) tree.cpp tree_generator.cpp -o tree
forest: forest.cpp tree_generator.hpp tree_generator.cpp
	g++ $(CPPFLAGS) forest.cpp tree_generator.cpp -o forest
debugF: forest.cpp tree_generator.hpp tree_generator.cpp test_util.cpp
	g++ $(CPPFLAGS_DBG) forest.cpp tree_generator.cpp test_util.cpp -o forest
debug: tree.cpp tree_generator.hpp tree_generator.cpp test_util.cpp test_util.hpp
	g++ $(CPPFLAGS_DBG) tree.cpp tree_generator.cpp test_util.cpp -o tree
generateCircle: generateCircle.cpp
	g++ $(CPPFLAGS) generateCircle.cpp -o genCircle
