///////////////////////////////////////////////////////////////////////////////
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// NetID:           rsharma54, mugundakrish
// CSLogin:         rsharma, mgiriprasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      November 24, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <sys/types.h>
#include <stdbool.h> 

typedef struct rbtree_node {
	struct rbtree_node *parent;
	struct rbtree_node *children[2];
	int key;
	int pid;
	int numAccess;
	unsigned long timeCreated;
	int clockBit;
	size_t size;
	int free;
	int red;
} rbtree_node;

rbtree_node rbtree_create(int key, int pid, unsigned long timeCreated);
int rbtree_insert(rbtree_node* node, int key, int pid, unsigned long timeCreated, bool maxMemReached);
void rbtree_delete_node(rbtree_node* node, int key);
void rbtree_delete_in_range(int key, size_t size);
void rbtree_free(rbtree_node* node);

rbtree_node *rbtree_node_search(int key);
rbtree_node *rbtree_interval_search(int key, int free);
rbtree_node *rbtree_range_search(int key, size_t size);

rbtree_node *searchForFIFO(rbtree_node *node);
rbtree_node *searchForLRU(rbtree_node *node);
rbtree_node *searchForClock(rbtree_node *node);

// These are used just in the script testing red black tree
void rbtree_print();
int is_red_black_tree();
