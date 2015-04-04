// map::key_comp
#include <iostream>
#include <map>
using namespace std;
struct Node
{
	int a;
	int b;

	bool operator<( const Node & n ) const {
		return this->a < n.a;   // for example
	}
};

struct Node2
{
	int a;
	int b;
};

int main ()
{
	cout << "with operator size:" << sizeof( Node ) << endl;
	cout << "without operator size:" << sizeof( Node2 ) << endl;
	
	return 0;
}
