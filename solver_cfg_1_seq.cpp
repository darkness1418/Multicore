#include <vector>
#include "board_cfg1.h"


#define MAX_GUESS_CELL_NUM 5


using namespace std;


vector<Board> getStartConfigs(Board &board, uint32_t maxGuessCellNum);
bool findSol(Board &board);


int main(int argc, char **argv)
{
    Board board;
    vector<Board> startConfigs;
    bool doesSolExist = false;
    size_t solIndex;

    // read a board layout
    cin >> board;

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
    startConfigs = getStartConfigs(board, MAX_GUESS_CELL_NUM);

    // find solution
    for (size_t i = 0; size_t < startConfigs.size(); ++size_t) {
        if (findSol(Board &board)) {
            doesSolExist = true;
            solIndex = i;
        }
    }

    // output solution or impossible
    cout << board;

    return 0;
}
