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

int pageSize = 4096; //bytes
int memorySize = 1048576; //mb
int maxNumNodes;
int currNumNodes = 0;
int prevPid;
int totalProcCount = 0;
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
	int currLineOffset = 0;
	while(!feof(file)) {
		fgets(currLine, 2*sizeof(char*), file);
		currLineOffset++;
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
			newProc->blocked = 0;
			newProc->firstLineOffset = currLineOffset;
			newProc->currLineOffset = currLineOffset;
			totalVpnArr[currPid] = newProc;
			totalProcCount++;
		} else {
			processInfo *newProc = totalVpnArr[currPid];
			newProc->finalVpn = currVpn;
			newProc->finalLineOffset = currLineOffset;
			newProc->totalNumVpn += 1;
		}
		
		updateTMR(1);
	}

	//second pass through actually runs through each process
	struct Queue *swapDrive = createQueue();
	int consecutiveNull = 0;
	int currentArrayIndex = 0;
	int totalBlockedCount = 0;
	int skipTime = 0;
	rbtree_node** procArr = calloc(100, sizeof(rbtree_node));
	while(consecutiveNull < 100) {
		if(currentArrayIndex >= 100) {
			currentArrayIndex = 0;
			if(totalBlockedCount == totalProcCount) {
				skipTime = 1;
			}
		}
		//check if there is stuff to remove from swap drive
		
		if(peek(swapDrive) != NULL) {
			int timeCheck = 2000000.0 == (getRT()-peek(swapDrive)->timeCreated);
			fprintf(stderr, "peek time %ld, curr rt %ld, time diff %ld, time check %d\n", peek(swapDrive)->timeCreated, getRT(), (getRT()-peek(swapDrive)->timeCreated), timeCheck);
			if(timeCheck || skipTime) {
				skipTime = 0;
				fprintf(stderr, "Got past swap drive check\n");
				//if the top of the queue has been in there for 2ms
				fprintf(stderr, "Swapping context\n");
				struct QueuePage *swapPage = dequeue(swapDrive);
				if(procArr[swapPage->pid] == NULL){
					fprintf(stderr, "creat tree from swap \n");
					procArr[swapPage->pid] = rbtree_create(swapPage->pid, swapPage->vpn, swapPage->timeCreated);
					currNumNodes++;
					totalVpnArr[currentArrayIndex]->blocked=0;
					totalBlockedCount--;
				} else {
					fprintf(stderr, "curr num nodes %d, max num nodes %d \n", currNumNodes, maxNumNodes);
					if(currNumNodes >= maxNumNodes) {
						procArr[swapPage->pid] = replace(procArr[swapPage->pid], swapPage->pid, swapPage->pid, swapPage->timeCreated);
					} else {
						procArr[swapPage->pid] = rbtree_insert(procArr[swapPage->pid], swapPage->vpn, swapPage->pid, swapPage->timeCreated, true);
						if(procArr[swapPage->pid]->insertResult == 1) {
							currNumNodes++;
						}	
					}
					totalVpnArr[currentArrayIndex]->blocked=0;
					totalBlockedCount--;
					updateRT(1.0); //1 ns
					prevPid = swapPage->pid;
					updateTotProcNum(1);
					processInfo *procInfo = totalVpnArr[swapPage->pid];
					procInfo->currNumVpn++;
					if(procInfo->currNumVpn == procInfo->totalNumVpn) {
						int procsFreed = 0;
						rbtree_free(procArr[swapPage->pid], &procsFreed);
						currNumNodes -= procsFreed;
					}
				}
			}
		}
		if(totalVpnArr[currentArrayIndex] == 0) {
			consecutiveNull++;
		} else if(totalVpnArr[currentArrayIndex] != 0) {
			fprintf(stderr, "Non null\n");
			consecutiveNull = 0;
			if(!totalVpnArr[currentArrayIndex]->blocked) {
				fprintf(stderr, "Not blocked\n");
				

				file = fopen(fileName, "r");
				if(file == NULL) {
					fprintf(stderr, "Error: Could not open file\n");
					exit(1);
				}
				//fseek to the line offset
				int currFileIndex = 0;
				fprintf(stderr, "pid %d and line offset %ld\n", currentArrayIndex, totalVpnArr[currentArrayIndex]->currLineOffset);
				while(!feof(file) && (currFileIndex < totalVpnArr[currentArrayIndex]->currLineOffset)) {
					fgets(currLine, 2*sizeof(char*), file);
					currFileIndex++;
				}
				fprintf(stderr, "%s\n", currLine);
				//parse its vpn
				currStringIndex = 0;

				//skip over prepended empty chars
				while(currLine[currStringIndex] == ' ') {
					currStringIndex++;
				}
				//skip the PID
				while(currLine[currStringIndex] != '\n' && currLine[currStringIndex] != ' ') {
					currStringIndex++;
				}
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

				fprintf(stderr, "Parsed vpn %d for pid %d\n", currVpn, currentArrayIndex);

				//check if pid is already in process array
				if(procArr[currentArrayIndex] == NULL) {
					fprintf(stderr, "Enqueue a completely new process\n");
					enqueue(swapDrive, currentArrayIndex, currVpn, getRT());
					updateTotProcNum(1);
					totalVpnArr[currentArrayIndex]->blocked=1;
					totalBlockedCount++;
				} else {
					//try inserting, then check for page fault
					fprintf(stderr, "curr num nodes %d, max num nodes %d \n", currNumNodes, maxNumNodes);
					procArr[currentArrayIndex] = rbtree_insert(procArr[currentArrayIndex], currVpn, currentArrayIndex, getRT(), false);
					int result = procArr[currentArrayIndex]->insertResult;
					if(result == 0) {
						fprintf(stderr, "Got to page fault \n");
						enqueue(swapDrive, currentArrayIndex, currVpn, getRT());
						updateTotProcNum(1);
						totalVpnArr[currentArrayIndex]->blocked=1;
						totalBlockedCount++;
					} else if(result == 2) {
						fprintf(stderr, "Got to duplicate \n");
						updateRT(1.0); //1 ns
						prevPid = currentArrayIndex;
						processInfo *procInfo = totalVpnArr[currentArrayIndex];
						procInfo->currNumVpn++;
						if(procInfo->currNumVpn == procInfo->totalNumVpn) {
							int procsFreed = 0;
							rbtree_free(procArr[currentArrayIndex], &procsFreed);
							totalVpnArr[currentArrayIndex] = 0;
							currNumNodes -= procsFreed;
						}
					}
				}
				totalVpnArr[currentArrayIndex]->currLineOffset++;
				fclose(file);
			}
		}
		currentArrayIndex++;
		updateRT(1.0);
	}
	free(totalVpnArr);
	free(swapDrive);
	free(currLine);
	printStats();
}