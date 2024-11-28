#ifndef LUDO_COMMON_H
#define LUDO_COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define BOARD_WIDTH 800
#define BOARD_HEIGHT 800
#define STATS_WIDTH 300
#define WINDOW_WIDTH (BOARD_WIDTH + STATS_WIDTH)
#define WINDOW_HEIGHT BOARD_HEIGHT
#define BOARD_SIZE 15
#define CELL_SIZE (BOARD_WIDTH / BOARD_SIZE)
#define TOKEN_SIZE (CELL_SIZE * 0.75)
#define DICE_SIZE 60
#define CIRCLE_RADIUS 50

typedef enum {
    EMPTY,
    RED_CELL,
    GREEN_CELL,
    BLUE_CELL,
    YELLOW_CELL,
    WHITE_CELL,
    STAR,
    HOME,
    CENTER,
    STARTING_CELL,
    RED_SAFE_CELL,
    GREEN_SAFE_CELL,
    YELLOW_SAFE_CELL,
    BLUE_SAFE_CELL
} CellType;

typedef struct {
    int x, y;
    SDL_Color color;
    bool moveable;
} Token;

extern CellType board[BOARD_SIZE][BOARD_SIZE];
extern Token tokens[16];
extern int tokenIndex;
extern int diceRolls[3];
extern int rollCount;
extern bool canRollAgain;
extern SDL_Texture* diceTextures[6];
extern int currentDiceRoll;
extern bool isRolling;
extern Uint32 rollStartTime;
extern bool StartGame;
extern int Players[4];
extern int PlayerTurn;
extern bool CLOCKWISE;
extern bool COUNTERCLOCKWISE;

#endif // LUDO_COMMON_H

