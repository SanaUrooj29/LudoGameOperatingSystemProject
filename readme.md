# LudoGameOperatingSystemProject

for running SDL 

sudo apt-get install libgraph-dev

sudo apt-get install libsdl2-dev

sudo apt-get install libsdl2-ttf-dev

sudo apt-get install libsdl2-gfx-dev

sudo apt-get install libsdl2-image-dev

To run the program (Run in Sequential Order):

gcc -c main.c ludo_board.c ludo_tokens.c ludo_dice.c ludo_render.c
gcc -o ludo_game main.c ludo_board.c ludo_tokens.c ludo_dice.c ludo_render.c -lSDL2 -lSDL2_image -lSDL2_ttf -lm
./ludo_game