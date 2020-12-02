CC=gcc
WARNING_FLAGS=-Wall -Wextra
SCAN_BUILD_DIR = scan-build-out
LIBS=-lpthread

537pfsim-fifo: main.o fileParser.o redblack_tree.o queue.o statsRecorder.o fifoReplacementPolicy.o
	$(CC) -g -o $(EXE) main.o fileParser.o redblack_tree.o queue.o statsRecorder.o fifoReplacementPolicy.o $(LIBS)
	
537pfsim-lru: main.o fileParser.o redblack_tree.o queue.o statsRecorder.o lruReplacementPolicy.o
	$(CC) -g -o $(EXE) main.o fileParser.o redblack_tree.o queue.o statsRecorder.o lruReplacementPolicy.o $(LIBS)
	
537pfsim-clock: main.o fileParser.o redblack_tree.o queue.o statsRecorder.o clockReplacementPolicy.o
	$(CC) -g -o $(EXE) main.o fileParser.o redblack_tree.o queue.o statsRecorder.o clockReplacementPolicy.o $(LIBS)

main.o: main.c fileParser.h statsRecorder.h
	$(CC) -g $(WARNING_FLAGS) -c main.c

fileParser.o: fileParser.c fileParser.h redblack_tree.h queue.h statsRecorder.h replacementPolicy.h
	$(CC) -g $(WARNING_FLAGS) -c fileParser.c

redblack_tree.o: redblack_tree.c redblack_tree.h
	$(CC) -g $(WARNING_FLAGS) -c redblack_tree.c
	
queue.o: queue.c queue.h
	$(CC) -g $(WARNING_FLAGS) -c queue.c
	
statsRecorder.o: statsRecorder.c statsRecorder.h
	$(CC) -g $(WARNING_FLAGS) -c statsRecorder.c

fifoReplacementPolicy.o: fifoReplacementPolicy.c replacementPolicy.h
	$(CC) -g $(WARNING_FLAGS) -c fifoReplacementPolicy.c

lruReplacementPolicy.o: lruReplacementPolicy.c replacementPolicy.h
	$(CC) -g $(WARNING_FLAGS) -c lruReplacementPolicy.c
	
clockReplacementPolicy.o: clockReplacementPolicy.c replacementPolicy.h
	$(CC) -g $(WARNING_FLAGS) -c clockReplacementPolicy.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

recompile: clean $(EXE)

