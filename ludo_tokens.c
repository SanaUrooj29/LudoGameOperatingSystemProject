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
            .color = {255, 0, 0, 255}
        };
    }

    // Green tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = {0, 255, 0, 255}
        };
    }

    // Blue tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {0, 0, 255, 255}
        };
    }

    // Yellow tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = {255, 255, 0, 255}
        };
    }
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