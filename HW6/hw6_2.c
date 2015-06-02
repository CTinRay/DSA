#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "bst.h"
#include "rb.h"

void preorder_integer_avl ( const struct avl_node*node ) {
	if (node== NULL)
		return ;
	printf( "%s " , (char*) node->avl_data ) ;
	if ( node->avl_link[0] != NULL || node -> avl_link[1] != NULL) {
		putchar ( '(' ) ;
			  preorder_integer_avl ( node -> avl_link[0] ) ;
			  putchar ( ',' ) ;
			  putchar ( ' ' ) ;
			  preorder_integer_avl ( node -> avl_link[1]) ;
			  putchar( ')' ) ;
	}
}

void preorder_integer_bst ( const struct bst_node*node ) {
	if (node== NULL)
		return ;
	printf( "%s " , (char*) node->bst_data ) ;
	if ( node->bst_link[0] != NULL || node -> bst_link[1] != NULL) {
		putchar ( '(' ) ;
			  preorder_integer_bst ( node -> bst_link[0] ) ;
			  putchar ( ',' ) ;
			  putchar ( ' ' ) ;
			  preorder_integer_bst ( node -> bst_link[1]) ;
			  putchar( ')' ) ;
	}
}

void preorder_integer_rb ( const struct rb_node*node ) {
	if (node== NULL)
		return ;
	printf( "%s " , (char*) node->rb_data ) ;
	if ( node->rb_link[0] != NULL || node -> rb_link[1] != NULL) {
		putchar ( '(' ) ;
			  preorder_integer_rb ( node -> rb_link[0] ) ;
			  putchar ( ',' ) ;
			  putchar ( ' ' ) ;
			  preorder_integer_rb ( node -> rb_link[1]) ;
			  putchar( ')' ) ;
	}
}


int str_compare ( const void *pa , const void *pb , void *param ){
	const char* strA = ( const char*) pa;
	const char* strB = ( const char*) pb ;
	return strcmp( strA, strB );
}

int main ( ) {
	struct avl_table*tree_avl = avl_create( str_compare, NULL, NULL );
	struct bst_table*tree_bst = bst_create( str_compare, NULL, NULL );
	struct  rb_table*tree_rb  =  rb_create( str_compare, NULL, NULL );
	int i ;
	for ( i =0; i < 32; i ++){
		char * str = (char*) malloc( sizeof(char) * 129);
		scanf( "%s", str );
		avl_probe( tree_avl, str );
		bst_probe( tree_bst, str );
		rb_probe(  tree_rb , str );
	}
	preorder_integer_avl( tree_avl -> avl_root);
	putchar( '\n' );
	preorder_integer_bst( tree_bst -> bst_root);
	putchar( '\n' );
	preorder_integer_rb( tree_rb -> rb_root);
	putchar( '\n' );
	return 0 ;
}
