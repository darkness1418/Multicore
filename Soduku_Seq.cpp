#include <iostream>
#include <vector>
#include <string>

class SudokuSolver {
public:
    void solveSudoku(std::vector<std::vector<std::string>>& board) {
        solve(board);
    }

private:
    bool solve(std::vector<std::vector<std::string>>& board) {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] == ".") {
                    for (int i = 1; i <= 9; ++i) {
                        std::string s_i = std::to_string(i);
                        if (isValid(board, s_i, r, c)) {
                            board[r][c] = s_i;
                            if (solve(board)) {
                                return true;
                            } else {
                                board[r][c] = ".";
                            }
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    bool isValid(const std::vector<std::vector<std::string>>& board, const std::string& i, int r, int c) {
        for (int n = 0; n < 9; ++n) {
            if (board[r][n] == i || board[n][c] == i || board[3 * (r / 3) + n / 3][3 * (c / 3) + n % 3] == i) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    // Example usage
    std::vector<std::vector<std::string>> board{
        {"5", "3", ".", ".", "7", ".", ".", ".", "."},
        {"6", ".", ".", "1", "9", "5", ".", ".", "."},
        {".", "9", "8", ".", ".", ".", ".", "6", "."},
        {"8", ".", ".", ".", "6", ".", ".", ".", "3"},
        {"4", ".", ".", "8", ".", "3", ".", ".", "1"},
        {"7", ".", ".", ".", "2", ".", ".", ".", "6"},
        {".", "6", ".", ".", ".", ".", "2", "8", "."},
        {".", ".", ".", "4", "1", "9", ".", ".", "5"},
        {".", ".", ".", ".", "8", ".", ".", "7", "9"}
    };

    SudokuSolver solver;
    solver.solveSudoku(board);

    // Print the solved board
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
