#include "ludo_common.h"
#include "ludo_dice.h"
#include "ludo_tokens.h"
#include "ludo_render.h"

int diceRolls[3] = {0, 0, 0};
int rollCount = 0;
bool canRollAgain = true;
SDL_Texture* diceTextures[6];
int currentDiceRoll = 1;
bool isRolling = false;
Uint32 rollStartTime = 0;

void loadDiceTextures(SDL_Renderer* renderer) {
    char path[50];
    for (int i = 1; i <= 6; i++) {
        sprintf(path, "assets/Dice/dieWhite_border%d.png", i);
        SDL_Surface* surface = IMG_Load(path);
        if (!surface) {
            printf("Failed to load dice image %d: %s\n", i, IMG_GetError());
            exit(1);
        }
        diceTextures[i-1] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}
void DiscardDiceRoll()
{
    diceRolls[0] = 0;
    diceRolls[1] = 0;
    diceRolls[2] = 0;
    rollCount = 0;
}
void handleDiceRoll() {
    SDL_Event event;
    if (canRollAgain) {
        currentDiceRoll = (rand() % 6) + 1;
        diceRolls[rollCount] = currentDiceRoll;
        rollCount++;

        if (currentDiceRoll != 6 || rollCount == 3) {
        printf("hellobro");
        }

        if (rollCount == 3 && diceRolls[0] == 6 && diceRolls[1] == 6 && diceRolls[2] == 6) {
            // Discard the rolls if three 6's are rolled
            DiscardDiceRoll();
            PlayerTurnSelection();
        }
    }
}

