////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: fileParser.h
// This File Description: This is the interface for the class that builds 
//                        specifications for the graph
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef PARSE_FILE
#define PARSE_FILEs

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "queue.h"
#include "replacementPolicy.h"

typedef struct processInfo {
	int finalVpn;
	long firstLineOffset;
	long finalLineOffset;
	long currLineOffset;
	int totalNumVpn;
    int currNumVpn;
	int blocked;
	rbtree_node* tree;
} processInfo;

void parseCommandLine(int argc, char* argv[]);

void parseFile();

#endif