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
void replace(rbtree_node *root, int pid, int vpn) {
    rbtree_node *temp_node = searchForLRU(root);
    rbtree_delete_node(root, temp_node->key);
    rbtree_insert(root, vpn, pid, getRT(), false);
}