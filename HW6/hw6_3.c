#include <stdio.h>
#include <stdlib.h>
#include "avl_ntudsa.h"
#include <assert.h>
#define MAX_GAMES 100001

typedef long long int lli;
typedef unsigned long long int ulli;


struct union_node{		
	int size;
	int belong;
	short root;
	int owner;
};


int max_game_cnt( struct avl_node* root, ulli money ){
	if( root == NULL ){
		return 0;
	}
	int left_cnt = 0;

	if( money < root -> avl_sum[0] ){ 
		return max_game_cnt( root -> avl_link[0] , money ); 
	}

	money -= root -> avl_sum[0];
	left_cnt += root -> avl_cnode[0];
	ulli node_sum = ( (ulli) root -> avl_data ) * ( root -> avl_cnt );

	if( money  >= node_sum ){
		money -= node_sum;
		left_cnt += root -> avl_cnt;
	}else{
		return left_cnt + ( money / root -> avl_data );
	}

	return max_game_cnt( root -> avl_link[1], money ) + left_cnt;
}

int union_find(struct union_node nodes[], int node_ind){
	/*Reminder: 
	  It seems to be that the size property is only useful for root node,
	  so I do not deal with the size of the nodeaffected on the path.
	*/
	int node_index_stake[MAX_GAMES];
	int stack_ind = 0;
	while( nodes[node_ind].belong != node_ind ){
		node_index_stake[stack_ind++] = node_ind;
		node_ind = nodes[node_ind].belong;
	}
	while( stack_ind > 0 ){
		int index = node_index_stake[--stack_ind];
		nodes[index].belong = node_ind;
	}
	return nodes[node_ind].owner;
}

int union_find_root_ind(struct union_node nodes[], int node_ind){
	/*Reminder: 
	  It seems to be that the size property is only useful for root node,
	  so I do not deal with the size of the nodeaffected on the path.
	*/
	int node_index_stake[MAX_GAMES];
	int stack_ind = 0;
	while( nodes[node_ind].belong != node_ind ){
		node_index_stake[stack_ind++] = node_ind;
		node_ind = nodes[node_ind].belong;
	}
	while( stack_ind > 0 ){
		int index = node_index_stake[--stack_ind];
		nodes[index].belong = node_ind;
	}
	return node_ind;
}


void union_merge(struct union_node nodes[], int receiver, int giver){

	int receiver_root = union_find_root_ind( nodes, receiver );
	int giver_root = union_find_root_ind( nodes, giver );

	int final_owner = nodes[ receiver_root ].owner;
	int final_root;
	int final_child;

	if( nodes[receiver_root].size < nodes[giver_root].size ){
		final_root = giver_root;
		final_child = receiver_root;
	}else{
		final_root = receiver_root;
		final_child = giver_root;
	}

	nodes[final_child].belong = final_root;
	nodes[final_root].size += nodes[final_child].size;		
	nodes[final_root].owner = final_owner;
	nodes[final_root].belong = final_root;
}	

void union_initial( struct union_node nodes[] ){
	for( int i = 0 ; i < MAX_GAMES ; ++i ){
		nodes[i].owner = i;
		nodes[i].belong = i;
		nodes[i].size = 1;
	}
}


void tree_merge(struct avl_table* tree1, struct avl_table* tree2 ){
	//printf("===========================\n");
	assert( tree1 != tree2 );
	if( tree1 == tree2 ){
		printf("ERROR\n");
	}
	struct avl_table *receiver, *giver;
	if( tree1 -> avl_count > tree2 -> avl_count ){
		giver = tree2;
		receiver = tree1;
	}else{
		giver = tree1;
		receiver = tree2;
	}
	struct avl_node *avl_stack[AVL_MAX_HEIGHT];	
	struct avl_node *node = giver -> avl_root;
	int depth = 0;       	
	while( depth >= 0 ){
		if( node -> avl_link[0] != NULL ){
			avl_stack[ depth ++ ] = node;
			struct avl_node* next_node = node -> avl_link[0];
			node -> avl_link[0] = NULL;
			node = next_node;
		}else if( node -> avl_link[1] != NULL ){
			avl_stack[ depth ++ ] = node;
			struct avl_node* next_node = node -> avl_link[1];
			node -> avl_link[1] = NULL;
			node = next_node;
		}else{			
			for( int i = 0 ; i < node -> avl_cnt ; ++i ){
				avl_probe( receiver, node -> avl_data );
			}
			//printf("%d %llu\n",node -> avl_data, (unsigned long long int) node );
			free( node );
			node = avl_stack[ --depth ];
		}
	}
	*tree1 = (*tree2 = *receiver);
}


int int_compare ( const void *pa , const void *pb , void *param ){
	if ( *(int*)pa > *(int*)pb ) return 1;
	if ( *(int*)pa == *(int*)pb ) return 0;
	return -1;
}

void printOwner(struct union_node nodes[], int nFriends){
	printf( "Belong Table:======================\n");
	for( int i = 1 ; i <= nFriends ; ++i ){
		int node_ind = i;
		while( nodes[node_ind].belong != node_ind ){
			node_ind = nodes[node_ind].belong;
		}
		printf( "%d \t %d \t %d \n", i, node_ind, nodes[node_ind].owner );
	}		
	printf( "===================================\n");
}
int main(){
	int nFriends, nIncidents;
	struct avl_table* avl_trees[MAX_GAMES];
	struct union_node game_owner_arr [MAX_GAMES];
		
	scanf( "%d %d", &nFriends, &nIncidents );
	union_initial( game_owner_arr );
	for( int i = 1 ; i <= nFriends ; ++i ){
		int price;
		scanf( "%d", &price );
		avl_trees[i] = avl_create (int_compare, NULL, NULL);
		avl_probe( avl_trees[i], price );
	}

	for( int i = 0 ; i < nIncidents ; ++i ){
		//printOwner( game_owner_arr, nFriends );
		int incident_type;
		scanf( "%d" , &incident_type );
		if( incident_type == 1 ){
			int tedious_game, interesting_game;
			scanf( "%d %d", &tedious_game, &interesting_game );
			int ind_requester = union_find_root_ind( game_owner_arr, tedious_game );
			int ind_provider = union_find_root_ind( game_owner_arr, interesting_game );
			if( ind_requester != ind_provider ){
				//	printf("Op: Merge %d to %d \n", interesting_game, tedious_game);
				union_merge( game_owner_arr, ind_requester, ind_provider );
				tree_merge( avl_trees[ind_requester], avl_trees[ind_provider] );
			}
		}else {
			int game;
			ulli money;
			scanf( "%d %llu", &game, &money );
			int owner = union_find( game_owner_arr, game );
			int root_ind = union_find_root_ind( game_owner_arr, game );
			printf( "%d %d\n", owner ,max_game_cnt( avl_trees[ root_ind ] -> avl_root, money ) );
		}
	}
}
