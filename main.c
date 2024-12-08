#include "ludo_common.h"
#include "ludo_board.h"
#include "ludo_tokens.h"
#include "ludo_dice.h"
#include "ludo_render.h"

bool StartGame = true;
bool CLOCKWISE = false;
bool COUNTERCLOCKWISE = false;
int PlayerTurn = 0;



int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ludo Board",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    loadDiceTextures(renderer);
    initializeBoard();
    initializeTokens();

    SDL_Surface* starSurface = IMG_Load("assets/star.png");
    if (!starSurface) {
        printf("Failed to load star image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture* starTexture = SDL_CreateTextureFromSurface(renderer, starSurface);
    SDL_FreeSurface(starSurface);

    SDL_Surface* redSurface = IMG_Load("assets/ludo_0/red_piece_trans.png");
    SDL_Surface* greenSurface = IMG_Load("assets/ludo_0/green_piece_trans.png");
    SDL_Surface* blueSurface = IMG_Load("assets/ludo_0/blue_piece_trans.png");
    SDL_Surface* yellowSurface = IMG_Load("assets/ludo_0/yellow_piece_trans.png");

    if (!redSurface || !greenSurface || !blueSurface || !yellowSurface) {
        printf("Failed to load token images: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture* redToken = SDL_CreateTextureFromSurface(renderer, redSurface);
    SDL_Texture* greenToken = SDL_CreateTextureFromSurface(renderer, greenSurface);
    SDL_Texture* blueToken = SDL_CreateTextureFromSurface(renderer, blueSurface);
    SDL_Texture* yellowToken = SDL_CreateTextureFromSurface(renderer, yellowSurface);

    SDL_FreeSurface(redSurface);
    SDL_FreeSurface(greenSurface);
    SDL_FreeSurface(blueSurface);
    SDL_FreeSurface(yellowSurface);

    bool quit = false;
    SDL_Event event;
    bool waitingForTokenSelection = false;

    // For Anyone Reading -> This while loop is the gameloop
    // PollEvents-> Listens to all the events in the game
    // Main Methods -> RenderClear() and RenderPresent()
    
    while (!quit && !isGameOver()) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            
            if (waitingForTokenSelection) {
                
                token_selection(&event, diceRolls);
                printf("hello");
                if (tokens[PlayerTurn * 4 - 4].moveable || tokens[PlayerTurn * 4 - 3].moveable || 
                    tokens[PlayerTurn * 4 - 2].moveable || tokens[PlayerTurn * 4 - 1].moveable) {
                    
                    waitingForTokenSelection = false;
                    DiscardDiceRoll();
                    PlayerTurnSelection();
                }
                else
                {
                    waitingForTokenSelection = false;
                    DiscardDiceRoll();
                    PlayerTurnSelection();
                }
            } else {
                int centerX = BOARD_WIDTH + STATS_WIDTH/2;
                int centerY = WINDOW_HEIGHT/2;
                int dx = mouseX - centerX;
                int dy = mouseY - centerY;
                if (dx*dx + dy*dy <= CIRCLE_RADIUS*CIRCLE_RADIUS) {
                    if (canRollAgain) {
                        isRolling = true;
                        rollStartTime = SDL_GetTicks();
                        printf("hello");
                    }
                }
            }
        }
    }

    if (isRolling) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - rollStartTime < 1000) {
            currentDiceRoll = (rand() % 6) + 1;
        } else {
            isRolling = false;
            handleDiceRoll();
            printf("wait");
            waitingForTokenSelection = true;
        }
    }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); //Clears the current rendering window 
        renderBoard(renderer, starTexture);
        renderStatistics(renderer);
        renderTokens(renderer, redToken, greenToken, blueToken, yellowToken);
        SDL_RenderPresent(renderer); //Renders all the changes since the past render call
        if (isGameOver()) {
            SDL_Delay(5000);  // Display the final result for 5 seconds before quitting
            quit = true;
        }
        
    }
    if (isGameOver()) {
        printf("Game Over!\n");
        for (int i = 0; i < 3; i++) {
            printf("%d place: Player %d\n", i + 1, winners[i] + 1);
        }
    }

    for (int i = 0; i < 6; i++) {
        SDL_DestroyTexture(diceTextures[i]);
    }
    SDL_DestroyTexture(starTexture);
    SDL_DestroyTexture(redToken);
    SDL_DestroyTexture(greenToken);
    SDL_DestroyTexture(blueToken);
    SDL_DestroyTexture(yellowToken);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

