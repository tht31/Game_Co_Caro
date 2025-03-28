#ifndef _LOGIC__H
#define _LOGIC__H

#include <ctime>

#define BOARD_SIZE 3
#define EMPTY_CELL ' '
#define O_CELL 'O'  // Robot
#define X_CELL 'X'  // Người chơi

struct Caro {
    char board[BOARD_SIZE][BOARD_SIZE];
    char nextMove = X_CELL; // Người chơi đi trước

    void init() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                board[i][j] = EMPTY_CELL;
    }

    bool isValidMove(int row, int col) {
        return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == EMPTY_CELL);
    }

    void move(int row, int col) {
        if (isValidMove(row, col)) {
            board[row][col] = nextMove;
            nextMove = (nextMove == X_CELL) ? O_CELL : X_CELL;
        }
    }

    bool checkWin(char player) {
        // Kiểm tra hàng
        for (int i = 0; i < BOARD_SIZE; i++)
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
                return true;

        // Kiểm tra cột
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
                return true;

        // Kiểm tra đường chéo chính
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
            return true;

        // Kiểm tra đường chéo phụ
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
            return true;

        return false;
    }

    bool isFull() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                if (board[i][j] == EMPTY_CELL)
                    return false;
        return true;
    }

    void robotMove() {
        if (nextMove != O_CELL) return; // Robot chỉ đi khi đến lượt

        srand(time(0));
        int row, col;
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (!isValidMove(row, col));

        move(row, col);
    }
};

#endif

