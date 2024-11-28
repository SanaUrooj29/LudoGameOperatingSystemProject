#ifndef LUDO_RENDER_H
#define LUDO_RENDER_H

#include "ludo_common.h"

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void drawFilledTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3);
void renderCenterTriangles(SDL_Renderer* renderer);
void renderBoard(SDL_Renderer* renderer, SDL_Texture* starTexture);
void renderTokens(SDL_Renderer* renderer, SDL_Texture* redToken, SDL_Texture* greenToken, SDL_Texture* blueToken, SDL_Texture* yellowToken);
void renderStatistics(SDL_Renderer* renderer);
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
void renderTextTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);
int PlayerTurnSelection();
void TurnDirection();

#endif // LUDO_RENDER_H

