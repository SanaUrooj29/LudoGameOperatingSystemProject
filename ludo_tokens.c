#include "ludo_common.h"
#include "ludo_tokens.h"
#include "ludo_board.h"

Token tokens[16];
int tokenIndex = 0;

// Define the paths for each color
const int PATH_LENGTH = 52;
const int MAIN_PATH[52][2] = {
    {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,5}, {6,4}, {6,3}, {6,2}, {6,1}, {6,0},
    {7,0}, {8,0}, {8,1}, {8,2}, {8,3}, {8,4}, {8,5}, {9,6}, {10,6}, {11,6}, {12,6},
    {13,6}, {14,6}, {14,7}, {14,8}, {13,8}, {12,8}, {11,8}, {10,8}, {9,8}, {8,9},
    {8,10}, {8,11}, {8,12}, {8,13}, {8,14}, {7,14}, {6,14}, {6,13}, {6,12}, {6,11},
    {6,10}, {6,9}, {5,8}, {4,8}, {3,8}, {2,8}, {1,8}, {0,8}, {0,7}, {0,6}
};

// Define home row paths for each color
const int HOME_ROW_LENGTH = 6;
const int RED_HOME_ROW[6][2] = {{1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7}};
const int GREEN_HOME_ROW[6][2] = {{7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}};
const int YELLOW_HOME_ROW[6][2] = {{13,7}, {12,7}, {11,7}, {10,7}, {9,7}, {8,7}};
const int BLUE_HOME_ROW[6][2] = {{7,13}, {7,12}, {7,11}, {7,10}, {7,9}, {7,8}};

void initializeTokens() {
    // Red tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = {255, 0, 0, 255},
            .moveable = false,
            .onBoard = false,
            .pathPosition = -1,
            .isCaptured = false // Added: Initialize isCaptured to false
        };
    }

    // Green tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = {0, 255, 0, 255},
            .moveable = false,
            .onBoard = false,
            .pathPosition = -1,
            .isCaptured = false // Added: Initialize isCaptured to false
        };
    }

    // Yellow tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {255, 255, 0, 255},
            .moveable = false,
            .onBoard = false,
            .pathPosition = -1,
            .isCaptured = false // Added: Initialize isCaptured to false
        };
    }

    // Blue tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {0, 0, 255, 255},
            .moveable = false,
            .onBoard = false,
            .pathPosition = -1,
            .isCaptured = false // Added: Initialize isCaptured to false
        };
    }
    for (int i = 0; i < 16; i++) {
        tokens[i].isCaptured = false;
    }
}

bool isInsideRect(int x, int y, SDL_Rect* rect) {
    return (x >= rect->x && x < rect->x + rect->w &&
            y >= rect->y && y < rect->y + rect->h);
}

void getPlayerTokenRange(int playerTurn, int* start, int* end) {
    *start = (playerTurn - 1) * 4;
    *end = *start + 4;
}

void getStartingPosition(SDL_Color color, int* startX, int* startY) {
    if (color.r == 255 && color.g == 0 && color.b == 0) {        // Red
        *startX = 1;
        *startY = 6;
    } 
    else if (color.r == 0 && color.g == 255 && color.b == 0) {   // Green
        *startX = 8;
        *startY = 1;
    } 
    else if (color.r == 255 && color.g == 255 && color.b == 0) { // Yellow
        *startX = 13;
        *startY = 8;
    } 
    else if (color.r == 0 && color.g == 0 && color.b == 255) {   // Blue
        *startX = 6;
        *startY = 13;
    }
}

int getStartingPathIndex(SDL_Color color) {
    if (color.r == 255 && color.g == 0 && color.b == 0) return 0;        // Red
    if (color.r == 0 && color.g == 255 && color.b == 0) return 13;       // Green
    if (color.r == 255 && color.g == 255 && color.b == 0) return 26;     // Yellow
    if (color.r == 0 && color.g == 0 && color.b == 255) return 39;       // Blue
    return 0; // Default to Red's starting position
}

bool can_move_token(Token* token, int diceRoll[]) {
    if (!token->onBoard) {
        // Token can only move onto the board if a 6 is rolled
        return (diceRoll[0] == 6 || diceRoll[1] == 6 || diceRoll[2] == 6);
    }
    // If the token is already on the board, it can always move
    return true;
}

void moveTokenAlongPath(Token* token, int steps) {
    int totalSteps = token->pathPosition + steps;
    
    // Check if the token is entering or moving within its home row
    if (totalSteps >= PATH_LENGTH) {
        int homeRowSteps = totalSteps - PATH_LENGTH;
        const int (*homeRow)[2];
        
        if (token->color.r == 255 && token->color.g == 0 && token->color.b == 0) homeRow = RED_HOME_ROW;
        else if (token->color.r == 0 && token->color.g == 255 && token->color.b == 0) homeRow = GREEN_HOME_ROW;
        else if (token->color.r == 255 && token->color.g == 255 && token->color.b == 0) homeRow = YELLOW_HOME_ROW;
        else homeRow = BLUE_HOME_ROW;
        
        if (homeRowSteps < HOME_ROW_LENGTH) {
            token->x = homeRow[homeRowSteps][0] * CELL_SIZE;
            token->y = homeRow[homeRowSteps][1] * CELL_SIZE;
            token->pathPosition = PATH_LENGTH + homeRowSteps;
        } else {
            // Token has reached or passed the final position in home row
            token->x = homeRow[HOME_ROW_LENGTH - 1][0] * CELL_SIZE;
            token->y = homeRow[HOME_ROW_LENGTH - 1][1] * CELL_SIZE;
            token->pathPosition = PATH_LENGTH + HOME_ROW_LENGTH - 1;
        }
    } else {
        // Move along the main path
        int newPosition = totalSteps % PATH_LENGTH;
        token->x = MAIN_PATH[newPosition][0] * CELL_SIZE;
        token->y = MAIN_PATH[newPosition][1] * CELL_SIZE;
        token->pathPosition = newPosition;
    }
    // After moving the token, check for captures
    for (int i = 0; i < 16; i++) {
        if (&tokens[i] != token && tokens[i].onBoard && !tokens[i].isCaptured &&
            tokens[i].x == token->x && tokens[i].y == token->y) {
            // Check if the current cell is not a safe cell
            int cellX = token->x / CELL_SIZE;
            int cellY = token->y / CELL_SIZE;
            if (board[cellY][cellX] != STAR && 
                board[cellY][cellX] != RED_SAFE_CELL && 
                board[cellY][cellX] != GREEN_SAFE_CELL && 
                board[cellY][cellX] != YELLOW_SAFE_CELL && 
                board[cellY][cellX] != BLUE_SAFE_CELL) {
                captureToken(&tokens[i]);
            }
        }
    }
}

void token_movement(int Tokens_Index, int diceRolls[]) {
    Token* token = &tokens[Tokens_Index];
    int totalSteps = 0;

    for (int i = 0; i < 3; i++) {
        if (diceRolls[i] != 0) {
            totalSteps += diceRolls[i];
        } else {
            break;
        }
    }

    if (!token->onBoard) {
        if (diceRolls[0] == 6 || diceRolls[1] == 6 || diceRolls[2] == 6) {
            int startX, startY;
            getStartingPosition(token->color, &startX, &startY);
            token->x = startX * CELL_SIZE;
            token->y = startY * CELL_SIZE;
            token->onBoard = true;
            token->pathPosition = getStartingPathIndex(token->color);
            totalSteps--;  // Reduce one step for moving onto the board
        } else {
            return;  // Can't move if not on board and didn't roll a 6
        }
    }

    if (totalSteps > 0) {
        moveTokenAlongPath(token, totalSteps);
    }

    // Reset dice rolls after movement
    for (int i = 0; i < 3; i++) {
        diceRolls[i] = 0;
    }
}

void captureToken(Token* capturedToken) {
    capturedToken->isCaptured = true;
    capturedToken->onBoard = false;
    capturedToken->pathPosition = -1;
    
    // Reset token to its initial position in the player's quadrant
    int playerIndex = (capturedToken - tokens) / 4;
    int tokenInPlayerIndex = (capturedToken - tokens) % 4;
    
    switch (playerIndex) {
        case 0: // Red
            capturedToken->x = CELL_SIZE * (tokenInPlayerIndex % 2 == 0 ? 1 : 4);
            capturedToken->y = CELL_SIZE * (tokenInPlayerIndex < 2 ? 1 : 4);
            break;
        case 1: // Green
            capturedToken->x = CELL_SIZE * (tokenInPlayerIndex % 2 == 0 ? 10 : 13);
            capturedToken->y = CELL_SIZE * (tokenInPlayerIndex < 2 ? 1 : 4);
            break;
        case 2: // Yellow
            capturedToken->x = CELL_SIZE * (tokenInPlayerIndex % 2 == 0 ? 10 : 13);
            capturedToken->y = CELL_SIZE * (tokenInPlayerIndex < 2 ? 10 : 13);
            break;
        case 3: // Blue
            capturedToken->x = CELL_SIZE * (tokenInPlayerIndex % 2 == 0 ? 1 : 4);
            capturedToken->y = CELL_SIZE * (tokenInPlayerIndex < 2 ? 10 : 13);
            break;
    }
}

void token_selection(SDL_Event* event, int diceRoll[]) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        int tokenStart, tokenEnd;
        getPlayerTokenRange(PlayerTurn, &tokenStart, &tokenEnd);

        for (int i = tokenStart; i < tokenEnd; i++) {
            SDL_Rect tokenRect = {
                tokens[i].x + (CELL_SIZE - TOKEN_SIZE) / 2,
                tokens[i].y + (CELL_SIZE - TOKEN_SIZE) / 2,
                TOKEN_SIZE,
                TOKEN_SIZE
            };

            if (isInsideRect(mouseX, mouseY, &tokenRect)) {
                if (can_move_token(&tokens[i], diceRoll)) {
                    tokens[i].moveable = true;
                    token_movement(i, diceRoll);
                    printf("Token %d selected and moved\n", i);
                    return;
                } else {
                    printf("This token cannot be moved\n");
                    return;
                }
            }
        }
    }
}