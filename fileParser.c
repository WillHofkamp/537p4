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
#include "statsRecorder.h"
#include "queue.h"
#include "replacementPolicy.h"

int replacementPolicy = 1; //1 = FIFO, 2 = LRU, 3 = CLOCK
int pageSize = 4096;
int memorySize = 1;
int maxNumNodes;
int currNumNodes = 0;
int prevPid;
char* fileName;

void parseCommandLine(int argc, const char* argv[]) {
	if(argc >  1) {
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
		maxNumNodes = memorySize/pageSize;
	}
}

void parseFile() {
	File* file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "Error: Could not open file\n");
		exit(1);
	}

	//first loop through the file gets all the pids and the final vpns of each process
	processInfo* totalVpnArr = malloc(100 * sizeof(processInfo));
	int currLineIndex = 0;
	char* currLine;
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

		if(totalVpnArr[currPid] == NULL) {
			processInfo *newProc = (processInfo *) malloc(sizeof(processInfo));
			newProc->finalVpn = currVpn;
			newProc->totalNumVpn = 1; 
			totalVpnArr[currPid] = newProc;
		} else {
			processInfo *newProc = totalVpnArr[currPid];
			newProc->finalVpn = currVpn;
			newProc->totalNumVpn += 1;
		}
		
		updateTMR(1);
	}

	//second pass through actually runs through each process
	Queue *swapDrive = createQueue();
	rbtree_node* procArr = malloc(100 * sizeof(rbtree_node));
	int currLineIndex = 0;
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
		
		//check if pid is already in process array
		if(procArr[currPid] == NULL || procArr[currPid] == false) {
			//check for page fault before creating
			if(currNumNodes >= maxNumNodes) {
				enqueue(swapDrive, currPid, currVpn);
				QueuePage *swapPage = dequeue(swapDrive);
				int swapVpn = swapPage->vpn;
				int swapPid = swapPage->pid;
				replace(procArr[prevPid], swapPid, swapVpn);
				updateTPI(1);
			} else {
				//create new tree if none
				procArr[currPid] = rbtree_create(currVpn, currPid, getRT());
				prevPid = currPid;
				currNumNodes++;
			}
			updateTotProcNum(1);
		} else {
			//try inserting, then check for page fault
			if(!rbtree_insert(procArr[currPid], currVpn, currPid, getRT(), currNumNodes >= maxNumNodes)); {
				enqueue(swapDrive, currPid, currVpn);
				QueuePage *swapPage = dequeue(swapDrive);
				int swapVpn = swapPage->vpn;
				int swapPid = swapPage->pid;
				replace(procArr[currPid], swapPid, swapVpn);
				currNumNodes--;
				updateTPI(1);
			}
			prevPid = currPid;
			currNumNodes++;
			totalVpnArr[currPid]->currVpn++;
			updateTotProcNum(1);
			if(totalVpnArr[currPid]->currNumVpn == totalVpnArr[currPid]->totalNumVpn) {
				rbtree_free(proccArr[currPid]);
			}
		}
	}
	
}