BLK_SIDE_SZ=4

.PHONY: all

all: test seq1

test: test.cpp board_cfg1.cpp board_cfg1.h Makefile
	g++ -DBLK_SIDE_SZ=${BLK_SIDE_SZ} -O3 -o test test.cpp board_cfg1.cpp

seq1: solver_cfg_1_seq.cpp board_cfg1.cpp board_cfg1.h Makefile
	g++ -DBLK_SIDE_SZ=${BLK_SIDE_SZ} -O3 -fopenmp -o seq1 solver_cfg_1_seq.cpp board_cfg1.cpp
