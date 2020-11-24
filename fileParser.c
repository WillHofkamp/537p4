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

#include "fileParser.h"

int replacementPolicy = 0; //1 = FIFO, 2 = LRU, 3 = CLOCK
int pageSize = 0;
int memorySize = 0;
char* fileName;

void parseCommandLine(int argc, const char* argv[]) {
	if(argc >  1) {
		if(strcmp(argv[0], "537pfsim-fifo") {
			replacementPolicy = 1;
		}else if(strcmp(argv[0], "537pfsim-lru") {
			replacementPolicy = 2;
		}else if(strcmp(argv[0], "537pfsim-clock") {
			replacementPolicy = 3;
		}
		
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-p") {
				i++;
				pageSize = argv[i];
			}else if(strcmp(argv[i], "-m") {
				i++;
				memorySize = argv[i];
			}else {
				fileName = argv[i];
			}
		} 
	}
}

void parseFile(){
	File* file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "Error: Could not open file\n");
		exit(1);
	}
	
	//first loop, get all pids and start and end of each pid
	while(!feof(file)) {
		//if pid isnt in array of processes
		
		
		//add new process to hash table
		
		
		
		
	}
	
}