#ifndef LUDO_TOKENS_H
#define LUDO_TOKENS_H

#include "ludo_common.h"

void initializeTokens();
void token_movement(int tokenIndex,int diceRolls[]);
void token_selection(SDL_Event *event, int diceRoll[]);
bool can_move_token(Token* token, int diceRoll[]);
#endif // LUDO_TOKENS_H

