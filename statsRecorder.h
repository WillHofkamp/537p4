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

#ifndef STATS_RECORDER
#define STATS_RECORDER

typedef struct StatsStruct StatsStruct;
struct StatsStruct {
	int totClockTicksNum;
	int totOccPageFramesNum;
	int totProcessNum;
	int totMemoryRefNum;
	int totPageFaultNum;
};

void updateAMU();

void updateARP();

void updateTMR();

void updateTPI();

void updateRT();

void printStats();

#endif