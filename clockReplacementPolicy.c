////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: clockReplacementPolicy.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "redblack_tree.h"
#include "replacementPolicy.h"

rbtree_node* currentPids = malloc(sizeof(rbtree_node) * 100);

/**
 * This will delete the oldest inserted node with the specified key
 */
void replace(rbtree_node *root) {
    if(currentPids[root->pid] != NULL) {
        rbtree_node *temp_node = searchForClock(currentPids[root->pid]);
        rbtree_delete_node(root, temp_node->key);
    } else {
        rbtree_node *temp_node = searchForClock(root);
        currentPids[root->pid] = temp_node; 
        rbtree_delete_node(root, temp_node->key);
    }
    
}