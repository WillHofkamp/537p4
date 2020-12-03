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

int replacementPolicy = 1; //1 = FIFO, 2 = LRU, 3 = CLOCK
int pageSize = 4096; //bytes
int memorySize = 1048576; //
int maxNumNodes;
int currNumNodes = 0;
int prevPid;
char* fileName;

void parseCommandLine(int argc, const char* argv[]) {
	if(argc >=  2) {
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-p") && argv[i+1] != NULL && isdigit(argv[i+1])) {
				i++;
				pageSize = atoi(argv[i]);
			}else if(strcmp(argv[i], "-m") && argv[i+1] != NULL && isdigit(argv[i+1])) {
				i++;
				memorySize = atoi(argv[i]);
				memorySize = memorySize >> 20;
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
	FILE* file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "Error: Could not open file\n");
		exit(1);
	}

	//first loop through the file gets all the pids and the final vpns of each process
	processInfo** totalVpnArr = calloc(100, sizeof(processInfo));
	char* currLine = malloc(sizeof(char*));
	int currStringIndex = 0;
	while(!feof(file)) {
		fgets(currLine, 2*sizeof(char*), file);
		currStringIndex = 0;

		//skip over prepended empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}
		//retrieve the PID
		char pidString[] = "";
		int pidStringLen = 0;
		while(currLine[currStringIndex] != '\n' && currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have PID with non-number\n");
				exit(1);
			}
			pidString[pidStringLen] = currLine[currStringIndex];
			pidStringLen++;
			pidString[pidStringLen] = '\0';
			currStringIndex++;
		}
		int currPid = atoi(pidString);

		//skip over more empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}
 
		//retrieve the VPN
		char vpnString[] = "";
		int vpnStringLen = 0;
		while(currLine[currStringIndex] != '\n' && currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have VPN with non-number \n");
				exit(1);
			}
			vpnString[vpnStringLen] = currLine[currStringIndex];
			vpnStringLen++;
			vpnString[vpnStringLen] = '\0';
			currStringIndex++;
		}
		int currVpn = atoi(vpnString);

		if(totalVpnArr[currPid] == 0) {
			processInfo *newProc = malloc(sizeof(processInfo));
			newProc->finalVpn = currVpn;
			newProc->totalNumVpn += 1;
			newProc->currNumVpn = 1;
			totalVpnArr[currPid] = newProc;
		} else {
			processInfo *newProc = totalVpnArr[currPid];
			newProc->finalVpn = currVpn;
			newProc->totalNumVpn += 1;
		}
		
		updateTMR(1);
	}

	//close the file then open again
	fclose(file);
	file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "Error: Could not open file\n");
		exit(1);
	}

	//second pass through actually runs through each process
	struct Queue *swapDrive = createQueue();
	rbtree_node** procArr = calloc(100, sizeof(rbtree_node));
	while(!feof(file)) {
		fgets(currLine, 2*sizeof(char*), file);
		currStringIndex = 0;
		//skip over prepended empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}

		fprintf(stderr, "Curr line %s", currLine);

		//retrieve the PID
		char pidString[] = "";
		int pidStringLen = 0;
		while(currLine[currStringIndex] != '\n' && currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have PID with non-number\n");
				exit(1);
			}
			pidString[pidStringLen] = currLine[currStringIndex];
			pidStringLen++;
			pidString[pidStringLen] = '\0';
			currStringIndex++;
		}
		int currPid = atoi(pidString);

		//skip over more empty chars
		while(currLine[currStringIndex] == ' ') {
			currStringIndex++;
		}
 
		//retrieve the VPN
		char vpnString[] = "";
		int vpnStringLen = 0;
		while(currLine[currStringIndex] != '\n' && currLine[currStringIndex] != ' ') {
			if(!isdigit(currLine[currStringIndex])) {
				fprintf(stderr, "Error: cannot have VPN with non-number \n");
				exit(1);
			}
			vpnString[vpnStringLen] = currLine[currStringIndex];
			vpnStringLen++;
			vpnString[vpnStringLen] = '\0';
			currStringIndex++;
		}
		int currVpn = atoi(vpnString);
		
		//check if pid is already in process array

		if(procArr[currPid] == 0) {
			//check for page fault before creating
			if(currNumNodes >= maxNumNodes) {
				enqueue(swapDrive, currPid, currVpn);
				struct QueuePage *swapPage = dequeue(swapDrive);
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
			if(!rbtree_insert(procArr[currPid], currVpn, currPid, getRT(), currNumNodes >= maxNumNodes)) {
				enqueue(swapDrive, currPid, currVpn);
				updateRT(2000000.0); //2ms
				struct QueuePage *swapPage = dequeue(swapDrive);
				int swapVpn = swapPage->vpn;
				int swapPid = swapPage->pid;
				replace(procArr[currPid], swapPid, swapVpn);
				currNumNodes--;
				updateTPI(1);
			}
			updateRT(1.0); //1 ns
			prevPid = currPid;
			currNumNodes++;
			updateTotProcNum(1);
			processInfo *procInfo = totalVpnArr[currPid];
			procInfo->currNumVpn++;
			if(procInfo->currNumVpn == procInfo->totalNumVpn) {
				fprintf(stderr, "Freeing the pid: %d\n", currPid);
				rbtree_free(procArr[currPid]);
			}
		}
	}
	
}