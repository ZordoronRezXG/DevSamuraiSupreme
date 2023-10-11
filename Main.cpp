/**
 * MIT License
 *
 * Copyright (c) 2023 Zordoron Rez
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * Contributions and Acknowledgments
 * -------------------------------
 *
 * We would appreciate and are grateful for any acknowledgments or shout-outs if you
 * use this software in your projects. While not a formal license requirement, we kindly
 * request that you consider giving credit to the original author, "Zordoron Rez",
 * when using or referencing this software.
 *
 * We believe in open-source collaboration and appreciate your support!
 */


#include <iostream>
#include <SDL.h>
#include "LogMaker.h"
LogMaker lm;
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Rect playerRect;
SDL_Renderer* renderer;
SDL_Texture* playerTexture;
SDL_Window* window;

bool isRunning = true;

// Function to load assets
bool loadAssets() {
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    if (!playerSurface) {
        cerr << "Failed to load player sprite. SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    return true;
}

// Initialize the game
void init() {
    lm.zlog(1,"SDL_INIT_EVERYTHING...");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    lm.zlog(1,"SDL_CreateWindow...");
    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Failed to create window. SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    lm.zlog(1,"SDL_CreateRendererr...");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Failed to create renderer. SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    lm.zlog(1,"Loading assets...");
    if (!loadAssets()) {
        isRunning = false;
        return;
    }

    playerRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32 }; // Assuming a fixed size for the player
    lm.zlog(1, "Finished Loading!");
}
// Handle game events
void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        }
    }
}

// Update the game state
void update() {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    const int playerSpeed = 2;  // Adjust the value as needed

    if (keyState[SDL_SCANCODE_UP]) {
        playerRect.y -= playerSpeed;
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        playerRect.y += playerSpeed;
    }
    if (keyState[SDL_SCANCODE_LEFT]) {
        playerRect.x -= playerSpeed;
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        playerRect.x += playerSpeed;
    }
}


// Render the game frame
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
    SDL_RenderPresent(renderer);
}

// Clean up the game
void cleanUp() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void TickGameLoop() {
    const int desiredFPS = 60;
    const int frameDelay = 1000 / desiredFPS; // Time in milliseconds per frame

    Uint32 frameStart, frameTime;

    while (isRunning) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    cleanUp();
}


int main(int argc, char* argv[]) {
    lm.zlog(1, "Initializing dependencies...");
    init();

    if (isRunning) {
        TickGameLoop();
    }

    return 0;
}
