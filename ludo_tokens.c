#include "ludo_common.h"
#include "ludo_tokens.h"
#include "ludo_board.h"
#include <stdbool.h>

Token tokens[16];
int tokenIndex = 0;

// Define the paths for each color
const int RED_PATH[57][2] = {
    {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,5}, {6,4}, {6,3}, {6,2}, {6,1}, {6,0},
    {7,0}, {8,0}, {8,1}, {8,2}, {8,3}, {8,4}, {8,5}, {9,6}, {10,6}, {11,6}, {12,6},
    {13,6}, {14,6}, {14,7}, {14,8}, {13,8}, {12,8}, {11,8}, {10,8}, {9,8}, {8,9},
    {8,10}, {8,11}, {8,12}, {8,13}, {8,14}, {7,14}, {6,14}, {6,13}, {6,12}, {6,11},
    {6,10}, {6,9}, {5,8}, {4,8}, {3,8}, {2,8}, {1,8}, {0,8}, {0,7}, 
    {1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7}
};

const int GREEN_PATH[57][2] = {
    {8,1}, {8,2}, {8,3}, {8,4}, {8,5}, {9,6}, {10,6}, {11,6}, {12,6}, {13,6}, {14,6},
    {14,7}, {14,8}, {13,8}, {12,8}, {11,8}, {10,8}, {9,8}, {8,9}, {8,10}, {8,11}, {8,12},
    {8,13}, {8,14}, {7,14}, {6,14}, {6,13}, {6,12}, {6,11}, {6,10}, {6,9}, {5,8},
    {4,8}, {3,8}, {2,8}, {1,8}, {0,8}, {0,7}, {0,6}, {1,6}, {2,6}, {3,6},
    {4,6}, {5,6}, {6,5}, {6,4}, {6,3}, {6,2}, {6,1}, {6,0}, {7,0}, 
    {7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}
};

const int YELLOW_PATH[57][2] = {
    {13,8}, {12,8}, {11,8}, {10,8}, {9,8}, {8,9}, {8,10}, {8,11}, {8,12}, {8,13}, {8,14},
    {7,14}, {6,14}, {6,13}, {6,12}, {6,11}, {6,10}, {6,9}, {5,8}, {4,8}, {3,8}, {2,8},
    {1,8}, {0,8}, {0,7}, {0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,5},
    {6,4}, {6,3}, {6,2}, {6,1}, {6,0}, {7,0}, {8,0}, {8,1}, {8,2}, {8,3},
    {8,4}, {8,5}, {9,6}, {10,6}, {11,6}, {12,6}, {13,6}, {14,6}, {14,7}, 
    {13,7}, {12,7}, {11,7}, {10,7}, {9,7}, {8,7}
};

const int BLUE_PATH[57][2] = {
    {6,13}, {6,12}, {6,11}, {6,10}, {6,9}, {5,8}, {4,8}, {3,8}, {2,8}, {1,8}, {0,8},
    {0,7}, {0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,5}, {6,4}, {6,3}, {6,2},
    {6,1}, {6,0}, {7,0}, {8,0}, {8,1}, {8,2}, {8,3}, {8,4}, {8,5}, {9,6},
    {10,6}, {11,6}, {12,6}, {13,6}, {14,6}, {14,7}, {14,8}, {13,8}, {12,8}, {11,8},
    {10,8}, {9,8}, {8,9}, {8,10}, {8,11}, {8,12}, {8,13}, {8,14}, {7,14}, 
    {7,13}, {7,12}, {7,11}, {7,10}, {7,9}, {7,8}
};



int winners[4] = {-1, -1, -1, -1};  
int winnerCount = 0;

// Add this function to check if a player has won
bool checkPlayerWin(int playerIndex) {
    int startRange, endRange;
    getPlayerTokenRange(playerIndex, &startRange, &endRange);
    
    for (int i = startRange; i < endRange; i++) {
        if (tokens[i].pathPosition != 57) {  // Assuming 57 is the final position in the home
            return false;
        }
    }
    return true;
}

// Add this function to handle player winning
void handlePlayerWin(int playerIndex) {
    if (winnerCount < 4) {
        winners[winnerCount] = playerIndex;
        winnerCount++;
        printf("Player %d has won! They are in position %d\n", playerIndex + 1, winnerCount);
    }
}
// Add this function to check if the game is over
bool isGameOver() {
    return winnerCount == 3;  // Game is over when 3 players have won
}

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
            .isCaptured = false ,// Added: Initialize isCaptured to false
            .hasCaptured = false // Added: Initialize isCaptured to false

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
            .isCaptured = false,
            .hasCaptured = false  // Added: Initialize isCaptured to false
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
            .isCaptured = false,
            .hasCaptured = false  // Added: Initialize isCaptured to false
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
            .isCaptured = false,
            .hasCaptured = false  // Added: Initialize isCaptured to false
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
int getStartingPathIndex(SDL_Color color) {
    return 0; // All paths now start at index 0
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

void moveTokenAlongPath(Token* token, int steps) {
    const int (*path)[2];
    int pathLength;

    // Select the appropriate path based on token color
    if (token->color.r == 255 && token->color.g == 0 && token->color.b == 0) {
        path = RED_PATH;
        pathLength = sizeof(RED_PATH) / sizeof(RED_PATH[0]);
    } else if (token->color.r == 0 && token->color.g == 255 && token->color.b == 0) {
        path = GREEN_PATH;
        pathLength = sizeof(GREEN_PATH) / sizeof(GREEN_PATH[0]);
    } else if (token->color.r == 255 && token->color.g == 255 && token->color.b == 0) {
        path = YELLOW_PATH;
        pathLength = sizeof(YELLOW_PATH) / sizeof(YELLOW_PATH[0]);
    } else {
        path = BLUE_PATH;
        pathLength = sizeof(BLUE_PATH) / sizeof(BLUE_PATH[0]);
    }

    int newPosition = token->pathPosition + steps;

    // Check if the new position is in or past the home row
    if (newPosition >= pathLength - 6) {
        if (!token->hasCaptured) {
            // If the token hasn't captured and is trying to enter home row, don't move
            return;
        }
        // If the token has captured, allow it to move, but cap it at the last position
        newPosition = (newPosition >= pathLength) ? pathLength - 1 : newPosition;
    }

    // Update token position
    token->pathPosition = newPosition;
    token->x = path[newPosition][0] * CELL_SIZE;
    token->y = path[newPosition][1] * CELL_SIZE;

    // Check for captures (existing code)
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
                captureToken(&tokens[i], i, PlayerTurn);
            }
        }
    }
     // Check if the player has won
    int playerIndex = (token - tokens) / 4;
    if (checkPlayerWin(playerIndex)) {
        handlePlayerWin(playerIndex);
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

    if (!token->onBoard) { // if no token for that player is onboard
        if (diceRolls[0] == 6 || diceRolls[1] == 6 || diceRolls[2] == 6) // if player rolls a 6 
        {
            int startX, startY;
            getStartingPosition(token->color, &startX, &startY);
            token->x = startX * CELL_SIZE;
            token->y = startY *CELL_SIZE;
            token->onBoard = true;
            token->pathPosition = getStartingPathIndex(token->color);
            return;
            
        } else {
            return;  // Can't move if not on board and didn't roll a 6
        }
    }

    if (totalSteps > 0 && can_move_token(token, diceRolls)) {
        moveTokenAlongPath(token, totalSteps);
    }

    // Reset dice rolls after movement
    for (int i = 0; i < 3; i++) {
        diceRolls[i] = 0;
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
                    token_movement(i, diceRoll);
                    return ;
                } else {
                    printf("This token cannot be moved\n");
                    return ;
                }
            }
        }
    }
    return ;
}
bool can_move_token(Token* token, int diceRoll[]) {
    if (!token->onBoard) {
        // Token can only move onto the board if a 6 is rolled
        return (diceRoll[0] == 6 || diceRoll[1] == 6 || diceRoll[2] == 6);
    }
    // If the token is already on the board, it can always move
    return true;
}

bool can_move_any_token(int playerTurn, int diceRoll[]) {
    int tokenStart, tokenEnd;
    getPlayerTokenRange(playerTurn, &tokenStart, &tokenEnd);

    for (int i = tokenStart; i < tokenEnd; i++) {
        if (can_move_token(&tokens[i], diceRoll)) { //Use the first dice roll
            return true;
        }
    }
    return false;
}

void captureToken(Token* capturedToken, int tokenindex,int PlayerTurn) {
    
    int startrange;
    int endrange;
    bool playersToken = false;

    getPlayerTokenRange(PlayerTurn, &startrange,&endrange);
    for(int i = startrange; i < endrange; i++)
    {
        if(i == tokenindex)
        {
            playersToken = true;
        }
    }

    if(playersToken == false)
    {
    int startRange, endRange;
    getPlayerTokenRange(PlayerTurn, &startRange, &endRange);
    for (int i = startRange; i < endRange; i++) {
        tokens[i].hasCaptured = true;
    }
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
}