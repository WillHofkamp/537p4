////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: 537make.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include "fileParser.h"
#include "statsRecorder.h"

int main(int argc, const char* argv[]) {
	
	parseCommandLine(argc, argv);
	parseFile();
	printStats();
	
}