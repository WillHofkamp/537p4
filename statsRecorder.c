////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: statsRecorder.h
// This File Description: This is the interface for the class that builds 
//                        specifications for the graph
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

int totOccPageFramesNum = 0;
int totProcessNum = 0;
int totMemoryRefNum = 0;
int totPageFaultNum = 0;
unsigned long clock;

void updateTotOccFrames(int amt) {
	totOccPageFramesNum += amt;
}

void updateTotProcNum(int amt) {
	totProcessNum += amt;
}

void updateTMR(int amt) {
	totMemoryRefNum += amt;
}

void updateTPI(int amt) {
	totPageFaultNum += amt;
}

void updateRT(int amt) {
	clock += amt;
}

int getAMU() {
	return totOccPageFramesNum/clock;
}

int getARP() {
	return totProcessNum/clock;
}

int getTMR() {
	return totMemoryRefNum;
}

int getTPI() {
	return totPageFaultNum;
}

unsigned long getRT() {
	return clock;
}

void printStats(){
	printf("Average Memory Utilization: %d\n", getAMU());
	printf("Average Runable Processes: %d\n", getARP());
	printf("Total Memory References: %d\n", getTMR());
	printf("Total Page Ins: %d\n", getTPI());
	printf("Final Running Time: %ld\n", getRT());
}