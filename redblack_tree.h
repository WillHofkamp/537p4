///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      November 24, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>

typedef struct rbtree_node {
	struct rbtree_node *parent;
	struct rbtree_node *children[2];
	int key;
	int pid;
	int numAccess;
  int queueOrder;
	size_t size;
	int free;
	int red;
} rbtree_node;

void rbtree_insert(int key, int pid, size_t size);
void rbtree_delete_node(int key);

void rbtree_delete_in_range(int key, size_t size);

rbtree_node *rbtree_node_search(int key);
rbtree_node *rbtree_interval_search(int key, int free);
rbtree_node *rbtree_range_search(int key, size_t size);

// These are used just in the script testing red black tree
void rbtree_print();
int is_red_black_tree();
