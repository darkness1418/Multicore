#pragma once

#include <cstdint>
#include <iostream>


#ifndef BLK_SIDE_SZ  // the number of elements on a side (row) of a block
#define BLK_SIDE_SZ 3
#endif

#define BLK_SZ (BLK_SIDE_SZ * BLK_SIDE_SZ)
#define EMPTY 0
#define MIN_CELL_VAL 1
#define MAX_CELL_VAL BLK_SZ

typedef uint8_t CellVal;
typedef uint8_t RowColCount;

#define ABS_ROW_COL(blkRowCol, offset) ((blkRowCol)*BLK_SIDE_SZ+(offset))
#define GET_BLK_ROW_COL(rowcol) ((rowcol)/BLK_SIDE_SZ)

#define STILL_POSSIBLE 0
#define IMPOSSIBLE 1

typedef int32_t PossibleStatus;


struct Board {
    // main data
    CellVal config[BLK_SZ][BLK_SZ];

    // annotation data
    bool fillRow[BLK_SZ][BLK_SZ+1];
    bool fillCol[BLK_SZ][BLK_SZ+1];
    bool fillBlk[BLK_SIDE_SZ][BLK_SIDE_SZ][BLK_SZ+1];
    bool changeSinceLast;
    bool possible;

    Board();

    void clearChange();
    void deduceOneCell(RowColCount row, RowColCount col);
    void deduceOneVal(CellVal checkVal);
    uint32_t getAllEmptyCellLocation(RowColCount rowLst[], RowColCount colLst[]);
    bool isChanged();
    bool isImmediatelyPossible();
    void setCellNoCheck(CellVal setVal, RowColCount row, RowColCount col);
    bool setCellCheck(CellVal setVal, RowColCount row, RowColCount col);
    void unsetCell(RowColCount row, RowColCount col);
    void validate();
};


std::istream &operator>>(std::istream &is, Board &board);
std::ostream &operator<<(std::ostream &os, Board &board);
