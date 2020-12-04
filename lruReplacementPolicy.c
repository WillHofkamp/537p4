////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: lruReplacementPolicy.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "replacementPolicy.h"

/**
 * This will delete the least recently used node with the specified key
 */
rbtree_node* replace(rbtree_node *root, unsigned long pid, unsigned long vpn, unsigned long timeCreated) {
    rbtree_node *temp_node = searchForLRU(root);
    root = rbtree_delete_node(root, temp_node->key);
    root = rbtree_insert(root, vpn, pid, timeCreated, true);
    return root;
}