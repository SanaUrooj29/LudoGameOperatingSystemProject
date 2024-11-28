#include "ludo_common.h"
#include "ludo_tokens.h"

Token tokens[16];
int tokenIndex = 0;


void initializeTokens() {
    // Red tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = {255, 0, 0, 255},
            .moveable = false
        };
    }

    // Green tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = {0, 255, 0, 255},
            .moveable = false
        };
    }

    // Blue tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {0, 0, 255, 255},
            .moveable = false
        };
    }

    // Yellow tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {255, 255, 0, 255},
            .moveable = false
        };
    }
}

bool isInsideRect(int x, int y, SDL_Rect* rect) {
    return (x >= rect->x && x < rect->x + rect->w &&
            y >= rect->y && y < rect->y + rect->h);
}

// Helper function to get the index range for the current player's tokens
void getPlayerTokenRange(int playerTurn, int* start, int* end) {
    *start = (playerTurn - 1) * 4;
    *end = *start + 4;
}

// Function to handle token selection
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
                // Check if the token can be moved based on game rules
                if (can_move_token(&tokens[i], diceRoll)) {
                    tokens[i].moveable = true;
                    token_movement(i, diceRoll);
                    printf("Token %d selected and moved\n", i);
                    return;  // Exit the function after selecting and moving a token
                } else {
                    printf("This token cannot be moved\n");
                }
            }
        }
    }
}

// Function to check if a token can be moved (implement game rules here)
bool can_move_token(Token* token, int diceRoll[]) {
    // Implement the rules for when a token can be moved
    // For example:
    // - If the token is in the starting area and the dice roll is 6
    // - If the token is already on the board
    // - Check if the move would be valid (not overshooting the end, etc.)
    
    // This is a placeholder implementation. Replace with actual game rules.
    return true;
}


void token_movement(int Tokens_Index,int diceRolls[])
{
    for(int i =0 ; i < 3 ;i++)
    {
        if(diceRolls[i] != 0)
        {
            tokens[Tokens_Index].x += diceRolls[i] * CELL_SIZE;
        }
        else if(diceRolls[i] == 0)
        {
            break;
        }
    }
}
