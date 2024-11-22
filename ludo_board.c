#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>


#define BOARD_WIDTH 800
#define BOARD_HEIGHT 800
#define STATS_WIDTH 200
#define WINDOW_WIDTH (BOARD_WIDTH + STATS_WIDTH)  
#define WINDOW_HEIGHT BOARD_HEIGHT
#define BOARD_SIZE 15
#define CELL_SIZE (BOARD_WIDTH / BOARD_SIZE)  
#define TOKEN_SIZE (CELL_SIZE * 0.75)
#define DICE_SIZE 60
#define CIRCLE_RADIUS 50

const SDL_Color RED = {255, 0, 0, 255};
const SDL_Color GREEN = {0, 255, 0, 255};
const SDL_Color BLUE = {0, 0, 255, 255};
const SDL_Color YELLOW = {255, 255, 0, 255};
const SDL_Color WHITE = {255, 255, 255, 255};

typedef enum {
    EMPTY,
    RED_CELL,
    GREEN_CELL,
    BLUE_CELL,
    YELLOW_CELL,
    WHITE_CELL,
    STAR,
    HOME,
    CENTER,
    STARTING_CELL,
    //the cells in which a token can enter the safe square:
    RED_SAFE_CELL,
    GREEN_SAFE_CELL,
    YELLOW_SAFE_CELL,
    BLUE_SAFE_CELL
} CellType;

// Token structure
typedef struct {
    int x, y; // Token position
    SDL_Color color;
} Token;

Token tokens[16]; // 4 tokens for each color
int tokenIndex = 0; // Keeps track of token initialization

CellType board[BOARD_SIZE][BOARD_SIZE];

int diceRolls[3] = {0, 0, 0};
int rollCount = 0;
bool canRollAgain = true;

SDL_Texture* diceTextures[6];
int currentDiceRoll = 1;
bool isRolling = false;
Uint32 rollStartTime = 0;

void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }

    // Set up colored regions
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            board[i][j] = RED_CELL;
            board[i][j+9] = GREEN_CELL;
            board[i+9][j] = BLUE_CELL;
            board[i+9][j+9] = YELLOW_CELL;
        }
    }

    // Set up paths
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[6][i] = WHITE_CELL;
        board[0][7] = WHITE_CELL;
        board[7][0] = WHITE_CELL;
        board[7][14] = WHITE_CELL;
        board[8][i] = WHITE_CELL;
        board[i][6] = WHITE_CELL;
        board[i][8] = WHITE_CELL;
        board[14][7] = WHITE_CELL;
    }

    // Set up center
    for (int i = 6; i <= 8; i++) {
        for (int j = 6; j <= 8; j++) {
            board[i][j] = CENTER;
        }
    }

    //SAFE CELLS
    for(int i = 1; i < 6; i++){
        board[7][i] = RED_SAFE_CELL;
        board[7][i+8] = YELLOW_SAFE_CELL;
        board[i][7] = GREEN_SAFE_CELL;
        board[i+8][7] = BLUE_SAFE_CELL;
    }

    // Set up stars
    board[2][6] = STAR;
    // board[6][2] = STAR;
    board[8][2] = STAR;
    // board[12][6] = STAR;
    board[6][12] = STAR;
    // board[8][12] = STAR;
    board[12][8] = STAR;

    // Set up home areas
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            board[i][j] = HOME;
            board[i][j+9] = HOME;
            board[i+9][j] = HOME;
            board[i+9][j+9] = HOME;
        }
    }

    //Starting Points of Tokens:

    board[6][1] = RED_CELL;
    board[13][6] = BLUE_CELL;
    board[1][8] = GREEN_CELL;
    board[8][13] = YELLOW_CELL;

}


void initializeTokens() {
    // Red tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = RED
        };
    }

    // Green tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 1 : 4),
            .color = GREEN
        };
    }

    // Blue tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 1 : 4),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = BLUE
        };
    }

    // Yellow tokens
    for (int i = 0; i < 4; i++) {
        tokens[tokenIndex++] = (Token){
            .x = CELL_SIZE * (i % 2 == 0 ? 10 : 13),
            .y = CELL_SIZE * (i < 2 ? 10 : 13),
            .color = YELLOW
        };
    }
}

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


void renderTokens(SDL_Renderer* renderer, SDL_Texture* redToken, SDL_Texture* greenToken, SDL_Texture* blueToken, SDL_Texture* yellowToken) {
    SDL_Texture* tokenTextures[] = {redToken, greenToken, blueToken, yellowToken};

    for (int i = 0; i < 16; i++) {
        SDL_Rect tokenRect = {
            tokens[i].x + (CELL_SIZE - TOKEN_SIZE) / 2,
            tokens[i].y + (CELL_SIZE - TOKEN_SIZE) / 2,
            TOKEN_SIZE,
            TOKEN_SIZE
        };

        SDL_Texture* currentTexture = tokenTextures[i / 4]; // Select texture based on token group
        SDL_RenderCopy(renderer, currentTexture, NULL, &tokenRect);
    }
}





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

void drawStar(SDL_Renderer* renderer, int centerX, int centerY, int size) {
    const int num_points = 6;
    int x[num_points], y[num_points];
    double angle = M_PI / 2;
    
    for (int i = 0; i < num_points; i++) {
        x[i] = centerX + size * cos(angle);
        y[i] = centerY + size * sin(angle);
        angle += (2 * M_PI / num_points);
    }

    for (int i = 0; i < num_points; i++) {
        SDL_RenderDrawLine(renderer, x[i], y[i], x[(i + num_points/2) % num_points], y[(i + num_points/2) % num_points]);
    }
}

// Function to draw a filled triangle
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


// Function to render the Ludo board's center triangles
void renderCenterTriangles(SDL_Renderer* renderer) {
    int centerStartX = 6 * CELL_SIZE;
    int centerStartY = 6 * CELL_SIZE;
    int centerEndX = 9 * CELL_SIZE;
    int centerEndY = 9 * CELL_SIZE;

    // Red triangle 
    SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b, GREEN.a);
    drawFilledTriangle(renderer, centerStartX, centerStartY, centerEndX, centerStartY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Green triangle 
    SDL_SetRenderDrawColor(renderer, YELLOW.r, YELLOW.g, YELLOW.b, YELLOW.a);
    drawFilledTriangle(renderer, centerEndX, centerStartY, centerEndX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Blue triangle
    SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, BLUE.a);
    drawFilledTriangle(renderer, centerStartX, centerEndY, centerEndX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);

    // Yellow triangle 
    SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
    drawFilledTriangle(renderer, centerStartX, centerStartY, centerStartX, centerEndY, (centerStartX + centerEndX) / 2, (centerStartY + centerEndY) / 2);
}


void renderBoard(SDL_Renderer* renderer, SDL_Texture* starTexture) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};

            switch (board[i][j]) {
                case RED_SAFE_CELL:
                case RED_CELL:
                    SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
                    break;
                case GREEN_SAFE_CELL:
                case GREEN_CELL:
                    SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b, GREEN.a);
                    break;
                case BLUE_SAFE_CELL:
                case BLUE_CELL:
                    SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, BLUE.a);
                    break;
                case YELLOW_SAFE_CELL:
                case YELLOW_CELL:
                    SDL_SetRenderDrawColor(renderer, YELLOW.r, YELLOW.g, YELLOW.b, YELLOW.a);
                    break;
                
                case WHITE_CELL:
                case HOME:
                case CENTER:
                    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            }
            
            SDL_RenderFillRect(renderer, &cell);
            
            // Draw cell borders only for white and star cells
            if (board[i][j] == WHITE_CELL || board[i][j] == STAR 
                || board[i][j] == RED_SAFE_CELL || board[i][j] == GREEN_SAFE_CELL 
                || board[i][j] == BLUE_SAFE_CELL || board[i][j] == YELLOW_SAFE_CELL) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &cell);
            }

            // Render stars
            if (board[i][j] == STAR) {
                SDL_Rect starRect = {
                    j * CELL_SIZE + (CELL_SIZE - CELL_SIZE * 0.75) / 2, // Center the star horizontally
                    i * CELL_SIZE + (CELL_SIZE - CELL_SIZE * 0.75) / 2, // Center the star vertically
                    CELL_SIZE * 0.75,                                   // Adjust size to fit the cell
                    CELL_SIZE * 0.75
                };
                SDL_RenderCopy(renderer, starTexture, NULL, &starRect);
            }
        }
        
    }

    renderCenterTriangles(renderer);
}

void renderStatistics(SDL_Renderer* renderer) {
    SDL_Rect statsRect = {BOARD_WIDTH, 0, STATS_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &statsRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &statsRect);

    TTF_Font* font = TTF_OpenFont("assets/dejavu-sans/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Statistics:", textColor);
    if (!surface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {BOARD_WIDTH + 10, 10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Display all dice rolls
    char buffer[50];
    sprintf(buffer, "Dice Rolls: %d %d %d", diceRolls[0], diceRolls[1], diceRolls[2]);
    surface = TTF_RenderText_Solid(font, buffer, (SDL_Color){RED.r, RED.g, RED.b, RED.a});
    if (!surface) {
        printf("Failed to render dice rolls text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create dice rolls texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    // Draw black circle
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

    // Draw dice
    SDL_Rect diceRect = {
        BOARD_WIDTH + STATS_WIDTH/2 - DICE_SIZE/2,
        WINDOW_HEIGHT/2 - DICE_SIZE/2,
        DICE_SIZE,
        DICE_SIZE
    };

    textRect.y += 80;
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_RenderCopy(renderer, diceTextures[currentDiceRoll - 1], NULL, &diceRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);
}

// functionality to handle dice rolling logic
void handleDiceRoll() {
    if (canRollAgain) {
        currentDiceRoll = (rand() % 6) + 1;
        diceRolls[rollCount] = currentDiceRoll;
        rollCount++;

        if (currentDiceRoll != 6 || rollCount == 3) {
            canRollAgain = false;
        }

        if (rollCount == 3 && diceRolls[0] == 6 && diceRolls[1] == 6 && diceRolls[2] == 6) {
            // Discard the rolls if three 6's are rolled
            diceRolls[0] = 0;
            diceRolls[1] = 0;
            diceRolls[2] = 0;
            rollCount = 0;
        }
    }
}

int main(int argc, char* argv[]) {

    srand(time(NULL));  // Seed the random number generator

    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
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

    // Load star image
    SDL_Surface* starSurface = IMG_Load("assets/star.png");
    if (!starSurface) {
        printf("Failed to load star image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture* starTexture = SDL_CreateTextureFromSurface(renderer, starSurface);
    SDL_FreeSurface(starSurface);

    // Load token images
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

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
                // Check if click is within the dice circle
                int centerX = BOARD_WIDTH + STATS_WIDTH/2;
                int centerY = WINDOW_HEIGHT/2;
                int dx = mouseX - centerX;
                int dy = mouseY - centerY;
                if (dx*dx + dy*dy <= CIRCLE_RADIUS*CIRCLE_RADIUS) {
                    if (canRollAgain) {
                        isRolling = true;
                        rollStartTime = SDL_GetTicks();
                    }
                }
            }
        }

        if (isRolling) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - rollStartTime < 1000) {
                // Roll the dice for 1 second
                currentDiceRoll = (rand() % 6) + 1;
            } else {
                isRolling = false;
                handleDiceRoll();
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderBoard(renderer, starTexture);
        renderStatistics(renderer);
        renderTokens(renderer, redToken, greenToken, blueToken, yellowToken);

        SDL_RenderPresent(renderer);
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
