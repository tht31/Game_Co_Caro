#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "graphics.h"
#include "defs.h"
#include "logic.h"

using namespace std;

void processClick(int x, int y, Caro& game, Graphics& graphics) {
    int clickedCol = x / (SCREEN_WIDTH / BOARD_SIZE);
    int clickedRow = y / (SCREEN_HEIGHT / BOARD_SIZE);

    if (game.isValidMove(clickedRow, clickedCol)) {
        game.move(clickedRow, clickedCol);
        graphics.render(game);

        char winner = EMPTY_CELL;
        if (graphics.isGameOver(game, winner)) {
            if (winner == X_CELL) {
                graphics.showMessage("You Win!");
            } else if (winner == O_CELL) {
                graphics.showMessage("You Lose!");
            } else {
                graphics.showMessage("It's a draw!");
            }
            return;
        }

        SDL_Delay(100);


        // Bot đi sau người chơi
        game.robotMove();
        graphics.render(game);

        if (graphics.isGameOver(game, winner)) {
            if (winner == X_CELL) {
                graphics.showMessage("You Win!");
            } else if (winner == O_CELL) {
                graphics.showMessage("You Lose!");
            } else {
                graphics.showMessage("It's a draw!");
            }
        }
    }
}

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    Mix_Music *gMusic = graphics.loadMusic("me-and-you-168379.mp3");
    graphics.play(gMusic);


    Caro game;
    game.init();
    graphics.render(game);

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                processClick(x, y, game, graphics);
                break;
        }
        SDL_Delay(100);
    }

    if (gMusic != nullptr) Mix_FreeMusic( gMusic );

    graphics.quit();
    return 0;
}


