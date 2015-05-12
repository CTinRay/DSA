CC=g++
FNAME=a1a.train
PNAME=a1a.test
EPSILON=0.05
T= 100
RM=rm -rf
CPPFLAGS = -O2 -std=c++11 -Wall 
CPPFLAGS_DBG = -std=c++11 -Wall -g3 -D_DEBUG

.PHONY: clean_tree_pred_func clean_forest_pred_func run_tree run_forest predict_tree predict_forest clean

tree_pred_func.cpp: tree $(FNAME)
	../tree $(FNAME) $(EPSILON) > $@

clean_tree_pred_func: 
	$(RM) tree_pred_func.cpp
forest_pred_func.cpp: forest $(FNAME)
	../forest $(FNAME) $(T) > $@

clean_forest_pred_func: 
	$(RM) forest_pred_func.cpp

predict_tree: tree_predictor.cpp tree_pred_func.cpp $(PNAME)
	$(CC) tree_predictor.cpp tree_pred_func.cpp -o tree_predictor
	../tree_predictor $(PNAME) > ./tree_prediction.txt

run_tree: clean_tree_pred_func predict_tree

predict_forest: forest_predictor.cpp forest_pred_func.cpp $(PNAME)
	$(CC) forest_predictor.cpp forest_pred_func.cpp -o forest_predictor
	../forest_predictor $(PNAME) > ./forest_prediction.txt

run_forest: clean_forest_pred_func predict_forest

clean:
	$(RM) tree_predictor forest_predictor tree_pred_func.cpp forest_pred_func.cpp

tree: ../tree.cpp ../tree_generator.hpp ../tree_generator.cpp
	g++ $(CPPFLAGS) ../tree.cpp ../tree_generator.cpp -o ../tree
forest: ../forest.cpp ../tree_generator.hpp ../tree_generator.cpp
	g++ $(CPPFLAGS) ../forest.cpp ../tree_generator.cpp -o ../forest
