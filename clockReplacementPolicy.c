////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: clockReplacementPolicy.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "replacementPolicy.h"

rbtree_node** currentPids;

/**
 * This will delete the oldest inserted node with the specified key
 */
rbtree_node* replace(rbtree_node *root, int pid, int vpn, unsigned long timeCreated) {
    currentPids = calloc(100, sizeof(rbtree_node));
    if(currentPids[root->pid] != 0) {
        rbtree_node *temp_node = searchForClock(currentPids[root->pid]);
        if(temp_node == NULL) {
            temp_node = searchForClock(root);
        }
        root = rbtree_delete_node(root, temp_node->key);
        root = rbtree_insert(root, vpn, pid, timeCreated, true);
        return root;
    } else {
        rbtree_node *temp_node = searchForClock(root);
        currentPids[root->pid] = temp_node; 
        root = rbtree_delete_node(root, temp_node->key);
        root = rbtree_insert(root, vpn, pid, timeCreated, true);
        return root;
    }
    
}