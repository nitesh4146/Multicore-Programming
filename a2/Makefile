CC=g++

CFLAGS=-o
CSTD=-std=c++11
PTH=-pthread

all: blocking.o non_block2.o intel_tbb.o
	$(CC) blocking.o non_block2.o intel_tbb.o -o hello

blocking.o: blocking.cpp
	$(CC) $(CSTD) blocking.cpp $(PTH) $(CFLAGS) blocking

non_block2.o: non_block2.cpp
	$(CC) $(CSTD) non_block2.cpp $(PTH) $(CFLAGS) non_block2

intel_tbb.o: intel_tbb.cpp
	$(CC) $(CSTD) intel_tbb.cpp $(PTH) $(CFLAGS) intel_tbb
