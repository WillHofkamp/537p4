////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: fifoReplacementPolicy.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "replacementPolicy.h"

/**
 * This will delete the oldest inserted node with the specified key
 */
rbtree_node* replace(rbtree_node *root, int pid, int vpn) {
    rbtree_node *temp_node = searchForFIFO(root);
    fprintf(stderr, "DELETEing the key: %d\n", temp_node->key);
    root = rbtree_delete_node(root, temp_node->key);
    root = rbtree_insert(root, vpn, pid, getRT(), false);
    return root;
}