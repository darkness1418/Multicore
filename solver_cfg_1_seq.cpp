#include <deque>
#include <omp.h>
#include <vector>
#include "board_cfg1.h"

#define MIN_START_CONFIG 12
#define NUM_THREADS 24

using namespace std;


vector<Board> getStartConfigs(Board &board, uint32_t minStartConfig);
bool findSolHelper(Board &board, uint32_t emptyCnt, RowColCount emptyRows[], RowColCount emptyCols[], uint32_t nextPos);
bool findSol(Board &board);


int main(int argc, char **argv)
{
    Board board;
    vector<Board> startConfigs;
    bool doesSolExist = false;
    size_t solIndex;

    // read a board layout
    cin >> board;

    cout << board << endl;

    // do a first round elimination
    for (;;) {
        board.clearChange();

        for (CellVal checkVal = MIN_CELL_VAL; checkVal <= MAX_CELL_VAL; ++checkVal) {
            board.deduceOneVal(checkVal);
        }

        if (!board.isImmediatelyPossible()) {
            break;
        }

        for (RowColCount row = 0; row < BLK_SZ; ++row) {
            for (RowColCount col = 0; col < BLK_SZ; ++col) {
                board.deduceOneCell(row, col);
            }
        }

        if (!board.isImmediatelyPossible()) {
            break;
        }

        if (!board.isChanged()) {
            board.clearChange();
            break;
        }
    }

    // at this point, we cannot conclude that there are no possibilities to fill in the sudoku board

    // generate different mutually exclusive configurations
    startConfigs = getStartConfigs(board, MIN_START_CONFIG);

    // find solution
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(dynamic)
    for (size_t i = 0; i < startConfigs.size(); ++i) {
        if (findSol(startConfigs[i])) {
            doesSolExist = true;
            solIndex = i;
        }
    }

    // output solution or impossible
    if (doesSolExist) {
        cout << startConfigs[solIndex] << endl;
    } else {
        cout << "Impossible." << endl;
    }

    return 0;
}


vector<Board> getStartConfigs(Board &board, uint32_t minStartConfig)
{
    deque<Board> res({board});  // contains a list of mutually exlusive configs that cover all possibilities
    RowColCount emptyRowLst[BLK_SZ * BLK_SZ];
    RowColCount emptyColLst[BLK_SZ * BLK_SZ];
    uint32_t emptyCnt;

    emptyCnt = board.getAllEmptyCellLocation(emptyRowLst, emptyColLst);

    for (;;) {
        RowColCount emptyRow, emptyCol;

        // check if we meet the minimum number of starting configs
        if (res.size() >= minStartConfig) {
            break;
        }

        // pick an empty cell to expand the number of starting configs
        if (emptyCnt == 0) {
            // no more empty cells
            break;
        }
        --emptyCnt;
        emptyRow = emptyRowLst[emptyCnt];
        emptyCol = emptyColLst[emptyCnt];

        // calculate new starting configs based on this cell choice
        for (size_t curQueueSz = res.size(); curQueueSz > 0; --curQueueSz) {
            Board &curBoard = res.front();

            for (CellVal tryVal = MIN_CELL_VAL; tryVal <= MAX_CELL_VAL; ++tryVal) {
                if (curBoard.setCellCheck(tryVal, emptyRow, emptyCol)) {
                    res.push_back(curBoard);
                    curBoard.unsetCell(emptyRow, emptyCol);
                }
            }

            res.pop_front();
        }
    }

    return vector<Board>(res.begin(), res.end());
}

bool findSolHelper(Board &board, uint32_t emptyCnt, RowColCount emptyRows[], RowColCount emptyCols[], uint32_t nextPos)
{
    int curEmptyRow, curEmptyCol;

    if (nextPos == emptyCnt) {
        // a solution is found
        return true;
    }

    curEmptyRow = emptyRows[nextPos];
    curEmptyCol = emptyCols[nextPos];

    if (board.config[curEmptyRow][curEmptyCol] != EMPTY) {
        return findSolHelper(board, emptyCnt, emptyRows, emptyCols, nextPos+1);
    }

    for (CellVal tryVal = MIN_CELL_VAL; tryVal <= MAX_CELL_VAL; ++tryVal) {
        if (board.setCellCheck(tryVal, curEmptyRow, curEmptyCol)) {
            Board save = board;

            // elimination
            for (;;) {
                board.clearChange();

                for (CellVal checkVal = MIN_CELL_VAL; checkVal <= MAX_CELL_VAL; ++checkVal) {
                    board.deduceOneVal(checkVal);
                }

                if (!board.isImmediatelyPossible()) {
                    goto CUR_VALUE_ATTEMPT_END;
                }

                for (RowColCount row = 0; row < BLK_SZ; ++row) {
                    for (RowColCount col = 0; col < BLK_SZ; ++col) {
                        board.deduceOneCell(row, col);
                    }
                }

                if (!board.isImmediatelyPossible()) {
                    goto CUR_VALUE_ATTEMPT_END;
                }

                if (!board.isChanged()) {
                    board.clearChange();
                    break;
                }
            }

            if (findSolHelper(board, emptyCnt, emptyRows, emptyCols, nextPos+1)) {
                return true;
            }

            CUR_VALUE_ATTEMPT_END:;

            board = save;
            board.unsetCell(curEmptyRow, curEmptyCol);
        }
    }

    return false;
}

bool findSol(Board &board)
{
    RowColCount emptyRows[BLK_SZ * BLK_SZ];
    RowColCount emptyCols[BLK_SZ * BLK_SZ];
    uint32_t emptyCnt;

    emptyCnt = board.getAllEmptyCellLocation(emptyRows, emptyCols);

    return findSolHelper(board, emptyCnt, emptyRows, emptyCols, 0);
}
