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
#include "redblack_tree.h"

int replacementPolicy = 1; //1 = FIFO, 2 = LRU, 3 = CLOCK
int pageSize = 4096;
int memorySize = 1;
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
			if(strcmp(argv[i], "-p") && argv[i+1] != NULL && isdigit(argv[i+1])) {
				i++;
				pageSize = atoi(argv[i]);
			}else if(strcmp(argv[i], "-m") && argv[i+1] != NULL && isdigit(argv[i+1])) {
				i++;
				memorySize = atoi(argv[i]);
			}else {
				fileName = argv[i];
			}
		}

		if(pageSize > memorySize) {
			fprintf(stderr, "Error: main memory size is lower than page size \n");
			exit(1);
		}
	}
}

void parseFile() {
	File* file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "Error: Could not open file\n");
		exit(1);
	}
	
	//one page table (queue) per process

	bool* procArr = malloc(100 * sizeof(bool));
	int currLineIndex = 0;
	char* currLine;
	//first loop, get all pids and start and end of each pid
	while(!feof(file)) {
		int currStringIndex = 0;
		fgets(currLine, currLineIndex, file);

		//skip over prepended empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}

		//retrieve the PID
		char* pidString;
		while(currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have PID with non-number \n");
				exit(1);
			}
			pidString += currLine[currStringIndex];
			currStringIndex++;
		}
		int currPid = atoi(pidString);

		//skip over more empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}
 
		//retrieve the VPN
		char* vpnString;
		while(currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have VPN with non-number \n");
				exit(1);
			}
			vpnString += currLine[currStringIndex];
			currStringIndex++;
		}
		int currVpn = atoi(vpnString);
		
		//create a new node from parsed values
		struct rbtree_node newNode = malloc(sizeof(rbtree_node));
		newNode->key= currVpn;
		newNode->pid = currPid;
		newNode->numAccess = 1;
		
		//check if pid is already in process array
		if(procArr[currPid] == NULL || procArr[currPid] == false) {
			//create new tree if not
			procArr[currPid] = true;
			rbtree_insert(currVpn, currPid, sizeof(currVpn) + sizeof(currPid));
			//increment totProcessNum
		} else {
			//check memory in tree
			//if tree is full, has hit max num nodes
			//replace(currVpn, currPid);


			rbtree_insert(currVpn, currPid, sizeof(currVpn) + sizeof(currPid));
		}
	}
	
}