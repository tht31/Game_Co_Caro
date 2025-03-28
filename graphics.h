#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "defs.h"
#include "logic.h"

struct Graphics {

     Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }
    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }

    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *cellEmpty, *cellX, *cellO;

    void init() {
        initSDL();
        cellEmpty = loadTexture("cell_empty.jpg");
        cellX = loadTexture("cell_x.png");
        cellO = loadTexture("cell_o.png");
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
            {
                logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                Mix_GetError() );
            }

    }

    void render(const Caro& game) {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++) {
                int x = j * (SCREEN_WIDTH / BOARD_SIZE);
                int y = i * (SCREEN_HEIGHT / BOARD_SIZE);
                switch (game.board[i][j]) {
                    case EMPTY_CELL: renderTexture(cellEmpty, x, y); break;
                    case X_CELL: renderTexture(cellX, x, y); break;
                    case O_CELL: renderTexture(cellO, x, y); break;
                }
            }
        presentScene();
    }

    void showMessage(const char* message) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message, window);
    }

    bool isGameOver(const Caro& game, char& winner) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (game.board[i][0] != EMPTY_CELL && game.board[i][0] == game.board[i][1] && game.board[i][1] == game.board[i][2]) {
                winner = game.board[i][0];
                return true;
            }
            if (game.board[0][i] != EMPTY_CELL && game.board[0][i] == game.board[1][i] && game.board[1][i] == game.board[2][i]) {
                winner = game.board[0][i];
                return true;
            }
        }
        if (game.board[0][0] != EMPTY_CELL && game.board[0][0] == game.board[1][1] && game.board[1][1] == game.board[2][2]) {
            winner = game.board[0][0];
            return true;
        }
        if (game.board[0][2] != EMPTY_CELL && game.board[0][2] == game.board[1][1] && game.board[1][1] == game.board[2][0]) {
            winner = game.board[0][2];
            return true;
        }
        return false;
    }

    void logErrorAndExit(const char* msg, const char* error) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void initSDL() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void presentScene() {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename) {
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y) {
        SDL_Rect dest = {x, y, SCREEN_WIDTH / BOARD_SIZE, SCREEN_HEIGHT / BOARD_SIZE};
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void quit() {
        Mix_Quit();
        SDL_DestroyTexture(cellEmpty);
        SDL_DestroyTexture(cellX);
        SDL_DestroyTexture(cellO);
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    }
};

#endif // _GRAPHICS__H


