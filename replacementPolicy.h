////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: replacementPolicy.h
// This File Description: 
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef REPLACEMENT_POLICY
#define REPLACEMENT_POLICY

#include "redblack_tree.h"
#include "statsRecorder.h"

rbtree_node* replace(rbtree_node *root, unsigned long pid, unsigned long vpn, unsigned long timeCreated);

#endif