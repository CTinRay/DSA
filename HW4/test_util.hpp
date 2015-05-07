#include <vector>
#include <string>
#include "tree_generator.hpp"
void printVec( std::vector<Example*>vec );
void printTree( Node*root , std::string indent );
int printOrder(std::map<Attr,std::vector<Example*> >orderByAttr);
