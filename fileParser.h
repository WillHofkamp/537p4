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
#include "queue.h"
#include "replacementPolicy.h"

typedef struct processInfo {
	int finalVpn;
	int totalNumVpn;
    int currNumVpn;
} processInfo;

void parseCommandLine(int argc, const char* argv[]);

void parseFile();

#endif