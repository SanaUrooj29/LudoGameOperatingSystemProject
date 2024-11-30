#include "ludo_common.h"
#include "ludo_board.h"

CellType board[BOARD_SIZE][BOARD_SIZE];

void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }

    // Set up colored regions
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            board[i][j] = RED_CELL;
            board[i][j+9] = GREEN_CELL;
            board[i+9][j] = BLUE_CELL;
            board[i+9][j+9] = YELLOW_CELL;
        }
    }

    // Set up paths
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[6][i] = WHITE_CELL;
        board[0][7] = WHITE_CELL;
        board[7][0] = WHITE_CELL;
        board[7][14] = WHITE_CELL;
        board[8][i] = WHITE_CELL;
        board[i][6] = WHITE_CELL;
        board[i][8] = WHITE_CELL;
        board[14][7] = WHITE_CELL;
    }

    // Set up center
    for (int i = 6; i <= 8; i++) {
        for (int j = 6; j <= 8; j++) {
            board[i][j] = CENTER;
        }
    }

    // SAFE CELLS
    for(int i = 1; i < 6; i++){
        board[7][i] = RED_SAFE_CELL;
        board[7][i+8] = YELLOW_SAFE_CELL;
        board[i][7] = GREEN_SAFE_CELL;
        board[i+8][7] = BLUE_SAFE_CELL;
    }

    // Set up stars
    board[2][6] = STAR;
    board[8][2] = STAR;
    board[6][12] = STAR;
    board[12][8] = STAR;

    // Set up home areas
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            board[i][j] = HOME;
            board[i][j+9] = HOME;
            board[i+9][j] = HOME;
            board[i+9][j+9] = HOME;
        }
    }

    // Starting Points of Tokens
    board[6][1] =RED_CELL ;
    board[13][6] = BLUE_CELL;
    board[1][8] = GREEN_CELL;
    board[8][13] = YELLOW_CELL;
}

