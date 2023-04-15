#include <cassert>
#include <cctype>
#include <cstring>
#include <iomanip>
#include "board_cfg1.h"


using namespace std;


Board::Board()
{
    memset(config, 0, sizeof config);

    memset(fillRow, 0, sizeof fillRow);
    memset(fillCol, 0, sizeof fillCol);
    memset(fillBlk, 0, sizeof fillBlk);
    changeSinceLast = false;
    possible = true;
}

void Board::clearChange()
{
    changeSinceLast = false;
}

void Board::deduceOneCell(RowColCount row, RowColCount col)
{
    RowColCount blkRow = GET_BLK_ROW_COL(row);
    RowColCount blkCol = GET_BLK_ROW_COL(col);
    CellVal finalVal = EMPTY;

    if (config[row][col] != EMPTY) {
        return;
    }

    for (CellVal tryVal = MIN_CELL_VAL; tryVal <= MAX_CELL_VAL; ++tryVal) {
        if (!fillRow[row][tryVal] && !fillCol[col][tryVal] && !fillBlk[blkRow][blkCol][tryVal]) {
            if (finalVal != EMPTY) {
                return;
            }
            finalVal = tryVal;
        }
    }

    if (finalVal == EMPTY) {
        possible = false;
        return;
    }

    setCellNoCheck(finalVal, row, col);
}

void Board::deduceOneVal(CellVal checkVal)
{
    // deduce each block
    for (RowColCount blkRow = 0; blkRow < BLK_SIDE_SZ; ++blkRow) {
        for (RowColCount blkCol = 0; blkCol < BLK_SIDE_SZ; ++blkCol) {
            bool curBlkImpossible[BLK_SIDE_SZ][BLK_SIDE_SZ];
            RowColCount emptyRow, emptyCol;
            uint32_t emptyCnt;

            if (fillBlk[blkRow][blkCol][checkVal]) {
                goto TRY_THIS_BLOCK_END;
            }

            for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                    curBlkImpossible[row][col] =
                        (config[ABS_ROW_COL(blkRow, row)][ABS_ROW_COL(blkCol, col)] != EMPTY);
                }
            }

            for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                if (fillRow[ABS_ROW_COL(blkRow, row)][checkVal]) {
                    for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                        curBlkImpossible[row][col] = true;
                    }
                }
            }

            for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                if (fillCol[ABS_ROW_COL(blkCol, col)][checkVal]) {
                    for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                        curBlkImpossible[row][col] = true;
                    }
                }
            }

            emptyCnt = 0;
            for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                    if (!curBlkImpossible[row][col]) {
                        if (emptyCnt >= 1) {
                            goto TRY_THIS_BLOCK_END;
                        }
                        ++emptyCnt;
                        emptyRow = ABS_ROW_COL(blkRow, row);
                        emptyCol = ABS_ROW_COL(blkCol, col);
                    }
                }
            }

            if (emptyCnt == 0) {
                // impossible configuration to fill up - need a value in a block but no cell to place it
                possible = false;
                return;
            }

            setCellNoCheck(checkVal, emptyRow, emptyCol);

            TRY_THIS_BLOCK_END:;
        }
    }
}

uint32_t Board::getAllEmptyCellLocation(RowColCount rowLst[], RowColCount colLst[])
{
    uint32_t emptyCnt = 0;

    for (RowColCount row = 0; row < BLK_SZ; ++row) {
        for (RowColCount col = 0; col < BLK_SZ; ++col) {
            if (config[row][col] == EMPTY) {
                rowLst[emptyCnt] = row;
                colLst[emptyCnt] = col;
                ++emptyCnt;
            }
        }
    }

    return emptyCnt;
}

bool Board::isChanged()
{
    return changeSinceLast;
}

bool Board::isImmediatelyPossible()
{
    return possible;
}

void Board::setCellNoCheck(CellVal setVal, RowColCount row, RowColCount col)
{
    assert(setVal >= MIN_CELL_VAL && setVal <= MAX_CELL_VAL);

    config[row][col] = setVal;

    fillRow[row][setVal] = true;
    fillCol[col][setVal] = true;
    fillBlk[GET_BLK_ROW_COL(row)][GET_BLK_ROW_COL(col)][setVal] = true;
    changeSinceLast = true;
}

bool Board::setCellCheck(CellVal setVal, RowColCount row, RowColCount col)
{
    if (
        setVal == EMPTY ||
        setVal < MIN_CELL_VAL ||
        setVal > MAX_CELL_VAL ||
        fillRow[row][setVal] ||
        fillCol[col][setVal] ||
        fillBlk[GET_BLK_ROW_COL(row)][GET_BLK_ROW_COL(col)][setVal]
    ) {
        return false;
    }

    setCellNoCheck(setVal, row, col);
    return true;
}

void Board::unsetCell(RowColCount row, RowColCount col)
{
    CellVal val = config[row][col];

    config[row][col] = EMPTY;

    fillRow[row][val] = false;
    fillCol[col][val] = false;
    fillBlk[GET_BLK_ROW_COL(row)][GET_BLK_ROW_COL(col)][val] = false;
}

void Board::validate()
{
    bool refFillRow[BLK_SZ][BLK_SZ+1];
    bool refFillCol[BLK_SZ][BLK_SZ+1];
    bool refFillBlk[BLK_SIDE_SZ][BLK_SIDE_SZ][BLK_SZ+1];

    memset(refFillRow, 0, sizeof refFillRow);
    memset(refFillCol, 0, sizeof refFillCol);
    memset(refFillBlk, 0, sizeof refFillBlk);

    for (RowColCount row = 0; row < BLK_SZ; ++row) {
        for (RowColCount col = 0; col < BLK_SZ; ++col) {
            CellVal curVal = config[row][col];
            if (curVal != EMPTY) {
                refFillRow[row][curVal] = true;
                refFillCol[col][curVal] = true;
                refFillBlk[GET_BLK_ROW_COL(row)][GET_BLK_ROW_COL(col)][curVal] = true;
            }
        }
    }

    assert(memcmp(fillRow, refFillRow, sizeof refFillRow) == 0);
    assert(memcmp(fillCol, refFillCol, sizeof refFillCol) == 0);
    assert(memcmp(fillBlk, refFillBlk, sizeof refFillBlk) == 0);
}


istream &operator>>(istream &is, Board &board)
{
    for (RowColCount row = 0; row < BLK_SZ; ++row) {
        for (RowColCount col = 0; col < BLK_SZ; ++col) {
            int nextVal;
            int nextChar;

            // ignore non-digit characters
            for (;;) {
                nextChar = is.peek();
                if (is.eof()) {
                    goto FINISH_READ;
                }
                if (isdigit(nextChar)) {
                    break;
                }
                is.ignore(1);
            }

            is >> nextVal;
            if ((CellVal) nextVal != EMPTY && !board.setCellCheck((CellVal) nextVal, row, col)) {
                goto FINISH_READ;
            }
        }
    }

    FINISH_READ:;

    return is;
}

ostream &operator<<(ostream &os, Board &board)
{
    uint32_t maxCellValLen = to_string(BLK_SZ).length();

    for (RowColCount row = 0; row < BLK_SZ; ++row) {
        for (RowColCount col = 0; col < BLK_SZ; ++col) {
            os << right << setw(maxCellValLen) << (uint32_t) board.config[row][col];
            if ((col+1) % BLK_SIDE_SZ == 0 && (col+1) < BLK_SZ) {
                os << " | ";
            } else {
                os << " ";
            }
        }
        os << endl;
        if ((row+1) % BLK_SIDE_SZ == 0 && (row+1) < BLK_SZ) {
            for (RowColCount col = 0; col < BLK_SZ; ++col) {
                os << right << string(maxCellValLen, '-');
                if ((col+1) % BLK_SIDE_SZ == 0 && (col+1) < BLK_SZ) {
                    os << "-+-";
                } else {
                    os << "-";
                }
            }
            os << endl;
        }
    }

    return os;
}
