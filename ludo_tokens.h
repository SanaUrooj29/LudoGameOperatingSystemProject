#ifndef LUDO_TOKENS_H
#define LUDO_TOKENS_H

#include "ludo_common.h"

void initializeTokens();
void token_movement(int Tokens_Index,int diceRolls[]);
void token_selection(SDL_Event *event, int diceRoll[]);
bool can_move_token(Token* token, int diceRoll[]);
void captureToken(Token* capturedToken, int tokenindex, int Playerturn);
bool can_move_any_token(int playerTurn, int diceRoll[]);
void getPlayerTokenRange(int playerTurn, int* start, int* end);
bool isInsideRect(int x, int y, SDL_Rect* rect);
#endif // LUDO_TOKENS_H

