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
#define PARSE_FILE

typedef struct Process{
	int* pid;
	int* start;
	int* end;
	int time;
};

void parseCommandLine(int argc, const char* argv[]);

void parseFile();

#endif