#include <iostream>
#include <vector>
#include <string>

class SudokuSolver {
public:
    void solveSudoku(std::vector<std::vector<std::string>>& board, int n) {
        solve(board, n);
    }

private:
    bool solve(std::vector<std::vector<std::string>>& board, int n) {
        for (int r = 0; r < n*n; ++r) {
            for (int c = 0; c < n*n; ++c) {
                if (board[r][c] == ".") {
                    for (int i = 1; i <= n*n; ++i) {
                        std::string s_i = std::to_string(i);
                        if (isValid(board, s_i, r, c, n)) {
                            board[r][c] = s_i;
                            if (solve(board, n)) {
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

    bool isValid(const std::vector<std::vector<std::string>>& board, const std::string& i, int r, int c, int n) {
        for (int k = 0; k < 9; ++k) {
            if (board[r][k] == i || board[k][c] == i || board[n * (r / n) + k / n][n * (c / n) + k % n] == i) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    // Example usage
    // std::vector<std::vector<std::string>> board{
    //     {"5", "3", ".", ".", "7", ".", ".", ".", "."},
    //     {"6", ".", ".", "1", "9", "5", ".", ".", "."},
    //     {".", "9", "8", ".", ".", ".", ".", "6", "."},
    //     {"8", ".", ".", ".", "6", ".", ".", ".", "3"},
    //     {"4", ".", ".", "8", ".", "3", ".", ".", "1"},
    //     {"7", ".", ".", ".", "2", ".", ".", ".", "6"},
    //     {".", "6", ".", ".", ".", ".", "2", "8", "."},
    //     {".", ".", ".", "4", "1", "9", ".", ".", "5"},
    //     {".", ".", ".", ".", "8", ".", ".", "7", "9"}
    // };

    // std::vector<std::vector<std::string>> invalid_sixteen_board{
    //     {".", ".", "4", ".", ".", ".", ".", "5", "16", "10", ".", "6", "9", ".", "14", "."},
    //     {"16", "12", "7", ".", ".", ".", "14", "8", ".", ".", ".", ".", ".", "4", ".", "6"},
    //     {".", ".", "2", ".", "16", "4", ".", ".", ".", ".", "3", "9", "12", ".", "7", "5"},
    //     {".", ".", "9", "11", "2", "10", "3", ".", "4", ".", "5", "12", ".", ".", ".", "15"},
    //     {"9", "7", ".", "16", "8", ".", ".", "2", "11", "1", ".", ".", ".", "3", ".", "."},
    //     {"13", ".", ".", ".", ".", ".", "6", "7", ".", ".", "10", ".", "2", ".", "1", "9"},
    //     {"10", ".", ".", ".", "4", "15", "11", "1", ".", ".", ".", ".", "16", "12", "8", "13"},
    //     {".", ".", "11", "4", "9", ".", "13", ".", "8", ".", ".", ".", ".", "5", ".", "10"},
    //     {"8", ".", ".", "6", ".", "14", "5", ".", ".", ".", "11", "16", ".", ".", "12", "."},
    //     {".", ".", ".", "2", "6", "8", "16", "4", ".", ".", ".", ".", ".", ".", ".", "1"},
    //     {".", ".", "1", ".", ".", "12", ".", ".", ".", ".", ".", ".", "8", ".", ".", "."},
    //     {".", ".", "15", "13", "7", ".", ".", "10", "3", ".", "6", "5", "14", ".", ".", "4"},
    //     {".", "5", ".", "12", ".", "1", "2", ".", "13", ".", ".", "7", ".", "10", "9", "16"},
    //     {"7", "3", ".", ".", ".", ".", "8", "14", ".", ".", "4", "2", "11", "1", "13", "12"},
    //     {".", ".", ".", "9", "13", ".", ".", ".", ".", ".", "1", "8", "15", ".", ".", "."},
    //     {"2", "15", ".", ".", ".", ".", ".", "12", "5", "6", ".", "16", "3", "14", "4", "8"}
    // };
    std::vector<std::vector<std::string>> sixteen_board{
    {".", "4", ".", ".", "8", ".", "16", ".", "11", ".", "1", ".", ".", "6", ".", "14"},
    {"3", ".", ".", "9", ".", "15", ".", "4", "10", "7", ".", "13", ".", ".", "12", "."},
    {".", "6", ".", ".", "2", "11", ".", ".", "8", ".", ".", ".", "3", ".", ".", "."},
    {"15", ".", ".", ".", ".", ".", ".", "13", ".", ".", "14", ".", ".", "2", ".", "4"},
    {".", ".", ".", "8", ".", "3", ".", ".", "7", ".", ".", ".", "5", ".", ".", "."},
    {".", ".", ".", ".", ".", ".", "14", ".", ".", ".", ".", "10", ".", "16", ".", "13"},
    {".", ".", "9", "2", "5", ".", ".", ".", ".", "8", ".", ".", "11", ".", ".", "."},
    {"12", ".", "1", ".", ".", ".", ".", "16", ".", ".", "13", ".", ".", ".", "3", "."},
    {".", ".", ".", "12", "15", ".", ".", ".", ".", "6", ".", ".", "13", ".", "4", "7"},
    {"16", "1", "13", ".", "10", ".", ".", "3", ".", ".", "7", ".", ".", "9", ".", "."},
    {"6", ".", ".", ".", ".", ".", "11", ".", "4", ".", ".", "9", ".", ".", "2", "10"},
    {".", "3", ".", ".", ".", "16", ".", ".", ".", "14", "8", ".", ".", "11", ".", "."},
    {"5", "11", "3", "10", ".", ".", ".", "8", ".", ".", "12", "4", ".", ".", ".", "1"},
    {".", "14", ".", ".", "11", "9", ".", ".", ".", "13", "15", ".", "4", ".", "5", "."},
    {"13", ".", "4", ".", "6", ".", ".", ".", ".", ".", ".", "2", ".", "12", ".", "9"},
    {".", "12", ".", "16", ".", "13", ".", ".", ".", "1", ".", ".", "6", ".", "15", "."},
};

    SudokuSolver solver;
    int n=4;
    solver.solveSudoku(sixteen_board, n);
    // Print the solved board
    for (const auto& row : sixteen_board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
