#include <iostream>
#include "board_cfg1.h"


using namespace std;


int main(int argc, char *argv[])
{
    Board board;

    cout << "BLK_SIDE_SZ = " << BLK_SIDE_SZ << endl;
    cout << "BLK_SZ = " << BLK_SZ << endl;

    cin >> board;

    cout << board;
    board.deduceOneVal(1);
    cout << board;

    return 0;
}
