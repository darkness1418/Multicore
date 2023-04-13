test: test.cpp board_cfg1.cpp board_cfg1.h Makefile
	g++ -DBLK_SIDE_SZ=2 -o test test.cpp board_cfg1.cpp

# seq1:
