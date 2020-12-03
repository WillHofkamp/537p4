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

#include <time.h>
#include <stdio.h>

void updateTotOccFrames(int amt);

void updateTotProcNum(int amt);

void updateTMR(int amt);

void updateTPI(int amt);

void updateRT(long amt);

int getAMU();

int getARP();

int getTMR();

int getTPI();

unsigned long getRT();

void printStats();

#endif