#include <utility>
#include <list>
#include <functional>
#include <cstring>
#include <iostream>
struct EmptyHeap: public std::exception {};

template<class T>
class BinomialHeap {
    private:
        /* inner class: binomial tree */
        struct BinomialTree {
            int _size;
            T element;
            std::list<BinomialTree*> children;

            BinomialTree(T _ele): _size(1), element(_ele) {
                children.clear();
            }

            /* return the size of BinomialTree
             * #note that nullptr->size() == 0
             * (nullptr is a null pointer (like NULL in C))
             */
            int size() {
                return (this!=nullptr)?this->_size:0;
            }
        };
        /* some definition for convinience
         */
        typedef BinomialTree BT;
        typedef BinomialHeap<T> BH;
        typedef std::pair<BT*, BT*> CarrySum;
        typedef std::pair<T, BH> MaxRemainder;

        /* Merge three binomial trees which have the same sizes
         *
         * INPUT:   a: operand a, b: operand b, c: carry in
         *          (each of them are either nullptr or a pointer of BinomialTree with size 2^n)
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: carry out (nullptr or a pointer of BinomialTree with size 2^(n+1))
         *          second: sum (nullptr or a pointer of BinomialTree with size 2^n)
         *
         * (just as a full adder: http://en.wikipedia.org/wiki/Adder_%28electronics%29#Full_adder )
         */
        CarrySum merge_tree(BT *a, BT *b, BT *c) {		
		BT*carry = nullptr;
		BT*sum = nullptr;
		std::function<void(BT*,BT*)>halfAdd = [&carry,&sum](BT* added,BT* adder ){
			if( added -> size() > 0 && adder -> size() > 0 ){
				if( added -> element > adder -> element ){
					added -> children.push_back( adder );
					carry = added;
					sum = nullptr;
				}else{
					adder -> children.push_back( added );
					carry = adder;
					sum = nullptr;
				}
				carry -> _size = added -> size() + adder -> size();
			}else{
			        sum = added != nullptr ? added : adder;
				if( sum != nullptr ){
					sum -> _size = added -> size() + adder -> size();
				}
			}
		};
		
		halfAdd( a, b );
		halfAdd( sum, c );
		CarrySum carrySum = std::make_pair( carry, sum );
		return carrySum;
        };

        /* Pop the maximum element of a binomial tree and make other elements a binomial heap.
         *
         * INPUT:   a: a pointer of BinomialTree
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: the maximum element
         *          second: a binomial heap consists of other elements
         */
        MaxRemainder pop_max(BT *bt) {
		MaxRemainder maxReminder;
		maxReminder.first = bt -> element;		
		BinomialHeap binomialHeap;
		int maxTreeIndex = 0;
		int i = 0;
		#ifdef _DEBUG
		std::cout << "bt -> children.size: " << bt -> children.size() << std::endl;
		#endif
		for( auto it = bt -> children.begin();
		     it != bt -> children.end(); ++it ){
			binomialHeap.trees[i] = (*it) ;
			if( (*it) -> size() > 0  &&
			    ( maxTreeIndex == -1 ||
			      (*it) -> element > binomialHeap.trees[maxTreeIndex] -> element) ){
				maxTreeIndex = i;
			}
			i += 1;
		}		    
		binomialHeap.size = bt -> size() -1 ;
		binomialHeap.maxTreeInd = maxTreeIndex;
		maxReminder.second = binomialHeap;
		bt -> children.clear();
		delete bt;
		return maxReminder;
        }

        int size;
        BT* trees[32]; //binomial trees of the binomial heap, where trees[i] is a tree with size 2^i.
	int maxTreeInd;
    public:
        BinomialHeap(): size(0) {
            for(int i=0; i<32; ++i) trees[i] = nullptr;
        }
        BinomialHeap(T element): size(1) {
            for(int i=0; i<32; ++i) trees[i] = nullptr;
            trees[0] = new BT(element);
	    maxTreeInd = 0;
        }

        /* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
         *
         * INPUT:   b: a pointer of BinomialHeap
         */
        void merge(BH &b) {
		size += b.size;
		BinomialTree*carry = nullptr;
		maxTreeInd = -1;
		for( int i = 0 ; i < 32 ; ++i ){
			CarrySum carrySum = merge_tree( trees[ i ], b.trees[ i ], carry );
			trees[ i ] = carrySum.second;
			if( trees[ i ] -> size() > 0 && 
			    ( maxTreeInd == -1 ||
			      trees[ i ] -> element > trees[ maxTreeInd ] -> element )){
				maxTreeInd = i;
			}
		        carry = carrySum.first;
		}
		#ifdef _DEBUG
		if( maxTreeInd == -1 ){
			std::cout << "mergeL:128 err! " << std::endl;
		}
		#endif
		b.size = 0;
		for( int i = 0 ; i < 31 ; ++i ){
			b.trees[ i ] = nullptr;
		}
        }
	int getSize(){
		return size;
	}
        void insert(const T &element) {
            BH tmp = BH(element);
            merge(tmp);
        }
	T top(){
		return trees[maxTreeInd] -> element;
	}		
        T pop() {
            if(size==0) throw EmptyHeap();
            else {
                //find the tree contains maximum element
                int max_tree = -1;
                for(int i=0; i<32; ++i){
                    if(trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element > trees[max_tree]->element))
                        max_tree = i;
		}
		#ifdef _DEBUG
		std::cout << "max_tree " << max_tree  << std::endl
			  << "heap size: " << size << std::endl;
		#endif
		size -= trees[max_tree] -> size();
                MaxRemainder m_r = pop_max(trees[max_tree]);
                T &max_element = m_r.first;
                BH &remainder = m_r.second;
		//************************

		//************************
                trees[max_tree] = nullptr;
                merge(remainder);
                return max_element;
            }
        }
};
