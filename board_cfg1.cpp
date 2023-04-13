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

void Board::deduceOneVal(CellVal checkVal)
{
    // deduce each block
    for (RowColCount blkRow = 0; blkRow < BLK_SIDE_SZ; ++blkRow) {
        for (RowColCount blkCol = 0; blkCol < BLK_SIDE_SZ; ++blkCol) {
            bool curBlkPossible[BLK_SIDE_SZ][BLK_SIDE_SZ];
            RowColCount emptyRow, emptyCol;
            uint32_t emptyCnt;

            memset(curBlkPossible, 0, sizeof curBlkPossible);

            cerr << "checking " << (int) blkRow << " " << (int) blkCol << endl;

            if (fillBlk[blkRow][blkCol][checkVal]) {
                goto TRY_THIS_BLOCK_END;
            }

            for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                if (fillRow[ABS_ROW_COL(blkRow, row)][checkVal]) {
                    for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                        curBlkPossible[row][col] = true;
                    }
                }
            }

            for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                if (fillCol[ABS_ROW_COL(blkCol, col)][checkVal]) {
                    for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                        curBlkPossible[row][col] = true;
                    }
                }
            }

            cerr << (int) blkRow << " " << (int) blkCol << endl;

            emptyCnt = 0;
            for (RowColCount row = 0; row < BLK_SIDE_SZ; ++row) {
                for (RowColCount col = 0; col < BLK_SIDE_SZ; ++col) {
                    if (!curBlkPossible[row][col]) {
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

            cout << "er ec " << (int) emptyRow << " " << (int) emptyCol << endl;
            setCellNoCheck(checkVal, emptyRow, emptyCol);

            TRY_THIS_BLOCK_END:;
        }
    }
}

void Board::setCellNoCheck(CellVal setVal, RowColCount row, RowColCount col)
{
    assert(setVal == EMPTY || setVal >= MIN_CELL_VAL && setVal <= MAX_CELL_VAL);
    config[row][col] = setVal;

    fillRow[row][setVal] = true;
    fillCol[col][setVal] = true;
    fillBlk[GET_BLK_ROW_COL(row)][GET_BLK_ROW_COL(col)][setVal] = true;
    changeSinceLast = true;
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
            board.setCellNoCheck((CellVal) nextVal, row, col);
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

/*


bool existInRow(Board &board, CellVal val, RowColCount blkRow)
{

}

bool existInCol(Board &board, CellVal val, RowColCount blkCol)
{

}

ConflictStatus deduceType1Blk(Board &board, CellVal checkVal, RowColCount blkRow, RowColCount blkCol)
{

}

ConflictStatus deduceType1Board(Board &board, CellVal checkVal)
{
    for (RowColCount blkRow = 0; blkRow < BLK_SIDE_SZ; ++blkRow) {
        for (RowColCount blkCol = 0; blkCol < BLK_SIDE_SZ; ++blkCol) {

        }
    }
}

ConflictStatus deduceType2Row(Board &board, RowColCount row)
{

}

ConflictStatus deduceType2Col(Board &board, RowColCount col)
{

} */
