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
int totalProgCount = 0;
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
			totalProgCount++;
		} else {
			processInfo *newProc = totalVpnArr[currPid];
			newProc->finalVpn = currVpn;
			newProc->finalLineOffset = currLineOffset;
			newProc->totalNumVpn += 1;
		}
		
		updateTMR(1);
	}

	fclose(file);

	//second pass through actually runs through each process
	struct Queue *swapDrive = createQueue();
	int currentArrayIndex = 0;
	int blockedCount = 0;
	rbtree_node** procArr = calloc(100, sizeof(rbtree_node));
	while(totalProgCount != 0) {
		if(currentArrayIndex >= 100) {
			currentArrayIndex = 0;
		}
		//check if there is stuff to remove from swap drive
		if(peek(swapDrive) != NULL) {
			int timeCheck = 2000000.0 <= (getRT()-peek(swapDrive)->timeCreated);
			//fprintf(stderr, "peek time %ld, curr rt %ld, time diff %ld, time check %d\n", peek(swapDrive)->timeCreated, getRT(), (getRT()-peek(swapDrive)->timeCreated), timeCheck);
			if(timeCheck) {
				//if the top of the queue has been in there for 2ms
				struct QueuePage *swapPage = dequeue(swapDrive);
				processInfo *procInfo = totalVpnArr[swapPage->pid];
				if(procArr[swapPage->pid] == NULL){
					//fprintf(stderr, "create tree from swap with pid %ld and vpn %ld\n", swapPage->pid, swapPage->vpn);
					procArr[swapPage->pid] = rbtree_create(swapPage->vpn, swapPage->pid, swapPage->timeCreated);
				} else {
					//fprintf(stderr, "add node to tree, pid %ld, vpn %ld, curr num nodes %d, max num nodes %d \n", swapPage->pid, swapPage->vpn, currNumNodes, maxNumNodes);
					if(currNumNodes >= maxNumNodes) {
						//fprintf(stderr, "Got to replacement pid %d\n", swapPage->pid);
						procArr[swapPage->pid] = replace(procArr[swapPage->pid], swapPage->pid, swapPage->vpn, swapPage->timeCreated);
						procInfo->currNumVpn--;
					} else {
						//fprintf(stderr, "Got to insert non root %d\n", swapPage->pid);
						procArr[swapPage->pid] = rbtree_insert(procArr[swapPage->pid], swapPage->vpn, swapPage->pid, swapPage->timeCreated, true);
						
					}
					procInfo->currNumVpn++;
				}
				totalVpnArr[swapPage->pid]->blocked=0;
				if(procInfo->currNumVpn == procInfo->totalNumVpn) {
					int procsFreed = 0;
					rbtree_free(procArr[swapPage->pid], &procsFreed);
					totalVpnArr[swapPage->pid] = 0;
					procArr[swapPage->pid] = 0;
					currNumNodes -= procsFreed;
					totalProgCount--;
				}
				updateRT(1.0); //1 ns
				prevPid = swapPage->pid;
				updateTotProcNum(1);
				currNumNodes++;
				blockedCount--;
			}
		}
		if((totalVpnArr[currentArrayIndex] != NULL || totalVpnArr[currentArrayIndex] != 0) && (totalVpnArr[currentArrayIndex]->currLineOffset <= totalVpnArr[currentArrayIndex]->finalLineOffset)) {
			if(!totalVpnArr[currentArrayIndex]->blocked) {
				file = fopen(fileName, "r");
				if(file == NULL) {
					fprintf(stderr, "Error: Could not open file\n");
					exit(1);
				}
				//fseek to the line offset
				//fprintf(stderr, "Pre file line search array index %ld\n", totalVpnArr[currentArrayIndex]->currLineOffset);
				int currFileIndex = 0;
				while(!feof(file) && (currFileIndex < totalVpnArr[currentArrayIndex]->currLineOffset)) {
					fgets(currLine, 2*sizeof(char*), file);
					currFileIndex++;
				}
				//fprintf(stderr, "Post file line search file index %d\n", currFileIndex);
				//fprintf(stderr, "\n %s\n", currLine);
				//fprintf(stderr, "curr pid %d, curr num nodes %d, max num nodes %d \n", currentArrayIndex, currNumNodes, maxNumNodes);
				//parse its vpn
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

				//fprintf(stderr, "curr pid %d, curr vpn %d, curr num nodes %d, max num nodes %d \n", currentArrayIndex, currVpn, currNumNodes, maxNumNodes);
				//check if pid is already in process array
				if(currPid != currentArrayIndex) {
					processInfo *procInfo = totalVpnArr[currentArrayIndex];
					if(procInfo->currNumVpn == procInfo->totalNumVpn) {
						totalVpnArr[currentArrayIndex]->currLineOffset--;
						int procsFreed = 0;
						rbtree_free(procArr[currentArrayIndex], &procsFreed);
						totalVpnArr[currentArrayIndex] = 0;
						procArr[currentArrayIndex] = 0;
						currNumNodes -= procsFreed;
						totalProgCount--;
					}
				} else if(procArr[currentArrayIndex] == NULL) {
					//fprintf(stderr, "Got to new tree\n");
					enqueue(swapDrive, currentArrayIndex, currVpn, getRT());
					updateTotProcNum(1);
					totalVpnArr[currentArrayIndex]->blocked=1;
					blockedCount++;
				} else {
					//try inserting, then check for page fault
					procArr[currentArrayIndex] = rbtree_insert(procArr[currentArrayIndex], currVpn, currentArrayIndex, getRT(), false);
					int result = procArr[currentArrayIndex]->insertResult;
					if(result == 0) {
						//fprintf(stderr, "Got to page fault \n");
						enqueue(swapDrive, currentArrayIndex, currVpn, getRT());
						updateTotProcNum(1);
						totalVpnArr[currentArrayIndex]->blocked=1;
						blockedCount++;
						updateTPI(1);
					} else if(result == 2) {
						updateRT(1.0); //1 ns
						prevPid = currentArrayIndex;
						processInfo *procInfo = totalVpnArr[currentArrayIndex];
						procInfo->currNumVpn++;
						//fprintf(stderr, "Got to duplicate, curr num vpn %d and total num vpn %d\n", procInfo->currNumVpn, procInfo->totalNumVpn);
						if(procInfo->currNumVpn == procInfo->totalNumVpn) {
							//fprintf(stderr, "DUP FREEING proc %d\n", currentArrayIndex);
							int procsFreed = 0;
							rbtree_free(procArr[currentArrayIndex], &procsFreed);
							totalVpnArr[currentArrayIndex] = 0;
							procArr[currentArrayIndex] = 0;
							currNumNodes -= procsFreed;
							totalProgCount--;
						}
					}
				}
				if(totalVpnArr[currentArrayIndex] != 0) {
					totalVpnArr[currentArrayIndex]->currLineOffset++;
				}	
				fclose(file);
			} else {
				if(totalProgCount == blockedCount) {
					unsigned long currTimeDiff = (getRT()-peek(swapDrive)->timeCreated) - 1.0;
					updateRT((2000000.0 - currTimeDiff));
				}
			}
		}
		currentArrayIndex++;
		updateRT(1.0);
	}
	free(totalVpnArr);
	free(swapDrive);
	free(currLine);
	printStats();
	exit(1);
}