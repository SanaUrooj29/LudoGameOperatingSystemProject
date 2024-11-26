#include "ludo_common.h"
#include "unistd.h"
#include "ludo_render.h"

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void drawFilledTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3) {
    int minY = SDL_min(y1, SDL_min(y2, y3));
    int maxY = SDL_max(y1, SDL_max(y2, y3));

    for (int y = minY; y <= maxY; y++) {
        int xStart = BOARD_WIDTH, xEnd = 0;

        // Check and calculate xStart and xEnd for each edge
        if (y2 != y1 && y >= SDL_min(y1, y2) && y <= SDL_max(y1, y2)) {
            int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
            xStart = SDL_min(xStart, x);
            xEnd = SDL_max(xEnd, x);
        }

        if (y3 != y2 && y >= SDL_min(y2, y3) && y <= SDL_max(y2, y3)) {
            int x = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
            xStart = SDL_min(xStart, x);
            xEnd = SDL_max(xEnd, x);
        }

        if (y3 != y1 && y >= SDL_min(y1, y3) && y <= SDL_max(y1, y3)) {
            int x = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
            xStart = SDL_min(xStart, x);
            xEnd = SDL_max(xEnd, x);
        }

        // Draw the horizontal line for the current scanline
        SDL_RenderDrawLine(renderer, xStart, y, xEnd, y);
    }
}

void renderCenterTriangles(SDL_Renderer* renderer) {
    int centerStartX = 6 * CELL_SIZE;
    int centerStartY = 6 * CELL_SIZE;
    int centerEndX = 9 * CELL_SIZE;
    int centerEndY = 9 * CELL_SIZE;

    // Red triangle 
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    drawFilledTriangle(renderer, centerStartX, centerStartY, centerEndX, centerStartY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Green triangle 
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    drawFilledTriangle(renderer, centerEndX, centerStartY, centerEndX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Blue triangle
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    drawFilledTriangle(renderer, centerStartX, centerEndY, centerEndX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Yellow triangle 
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawFilledTriangle(renderer, centerStartX, centerStartY, centerStartX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);
}

void renderBoard(SDL_Renderer* renderer, SDL_Texture* starTexture) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};

            switch (board[i][j]) {
                case RED_SAFE_CELL:
                case RED_CELL:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case GREEN_SAFE_CELL:
                case GREEN_CELL:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case BLUE_SAFE_CELL:
                case BLUE_CELL:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                case YELLOW_SAFE_CELL:
                case YELLOW_CELL:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    break;
                case WHITE_CELL:
                case HOME:
                case CENTER:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            }
            
            SDL_RenderFillRect(renderer, &cell);
            
            if (board[i][j] == WHITE_CELL || board[i][j] == STAR 
                || board[i][j] == RED_SAFE_CELL || board[i][j] == GREEN_SAFE_CELL 
                || board[i][j] == BLUE_SAFE_CELL || board[i][j] == YELLOW_SAFE_CELL) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &cell);
            }

            if (board[i][j] == STAR) {
                SDL_Rect starRect = {
                    j * CELL_SIZE + (CELL_SIZE - CELL_SIZE * 0.75) / 2,
                    i * CELL_SIZE + (CELL_SIZE - CELL_SIZE * 0.75) / 2,
                    CELL_SIZE *0.75,
                    CELL_SIZE * 0.75
                };
                SDL_RenderCopy(renderer, starTexture, NULL, &starRect);
            }
        }
    }

    renderCenterTriangles(renderer);
}

void renderTokens(SDL_Renderer* renderer, SDL_Texture* redToken, SDL_Texture* greenToken, SDL_Texture* blueToken, SDL_Texture* yellowToken) {
    SDL_Texture* tokenTextures[] = {redToken, greenToken, blueToken, yellowToken};

    for (int i = 0; i < 16; i++) {
        SDL_Rect tokenRect = {
            tokens[i].x + (CELL_SIZE - TOKEN_SIZE) / 2,
            tokens[i].y + (CELL_SIZE - TOKEN_SIZE) / 2,
            TOKEN_SIZE,
            TOKEN_SIZE
        };

        SDL_Texture* currentTexture = tokenTextures[i / 4];
        SDL_RenderCopy(renderer, currentTexture, NULL, &tokenRect);
    }
}

// This function is responsible for rendering a texture for text 
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_FreeSurface(surface);
    return texture;
}
// This function is responsible for the rendering the text
void renderTextTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
    if (!texture) {
        return;
    }

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

void renderStatistics(SDL_Renderer* renderer) {
    static Uint32 startTime = 0;
    static int startupPhase = 0;

    SDL_Rect statsRect = {BOARD_WIDTH, 0, STATS_WIDTH + 100, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &statsRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &statsRect);

    TTF_Font* font = TTF_OpenFont("assets/dejavu-sans/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    if(StartGame) {
        if (startTime == 0) {
            startTime = SDL_GetTicks();
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        if (elapsedTime < 3000) {  // Show "Game Starting" for 3 seconds
            SDL_Color textColor0 = {0, 0, 0, 255};
            SDL_Texture* GameStart = renderText(renderer, font, "Game Starting:", textColor0);
            renderTextTexture(renderer, GameStart, BOARD_WIDTH + 10, 10);
            SDL_DestroyTexture(GameStart);
        } else if (elapsedTime < 6000) {  // Show player selection for another 3 seconds
            char playerbuffer[50];
            sprintf(playerbuffer, "Selecting Player Randomly: %d", 2);
            SDL_Color textColor1 = {0, 0, 0, 255};
            SDL_Texture* PlayerSelection = renderText(renderer, font, playerbuffer, textColor1);
            renderTextTexture(renderer, PlayerSelection, BOARD_WIDTH + 10, 30);
            SDL_DestroyTexture(PlayerSelection);
        } else {
            StartGame = false;
            startTime = 0;
        }
    } else {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Texture* titleTexture = renderText(renderer, font, "Statistics:", textColor);
        renderTextTexture(renderer, titleTexture, BOARD_WIDTH + 10, 10);

        char buffer[50];
        sprintf(buffer, "Dice Rolls: %d %d %d", diceRolls[0], diceRolls[1], diceRolls[2]);
        SDL_Color diceRollColor = {255, 0, 0, 255};
        SDL_Texture* diceRollTexture = renderText(renderer, font, buffer, diceRollColor);
        renderTextTexture(renderer, diceRollTexture, BOARD_WIDTH + 10, 90);

        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(diceRollTexture);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int y = -CIRCLE_RADIUS; y <= CIRCLE_RADIUS; y++) {
        for (int x = -CIRCLE_RADIUS; x <= CIRCLE_RADIUS; x++) {
            if (x*x + y*y <= CIRCLE_RADIUS*CIRCLE_RADIUS) {
                SDL_RenderDrawPoint(renderer, 
                    BOARD_WIDTH + STATS_WIDTH/2 + x, 
                    WINDOW_HEIGHT/2 + y);
            }
        }
    }

    SDL_Rect diceRect = {
        BOARD_WIDTH + STATS_WIDTH/2 - DICE_SIZE/2,
        WINDOW_HEIGHT/2 - DICE_SIZE/2,
        DICE_SIZE,
        DICE_SIZE
    };

    SDL_RenderCopy(renderer, diceTextures[currentDiceRoll - 1], NULL, &diceRect);
    TTF_CloseFont(font);
}

