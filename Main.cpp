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
//hello

#include <iostream>
#include <SDL.h>

#include <map>
#include <fstream>
#include "json.hpp"
//to do
#include <SDL_ttf.h>

#include "LogMaker.h"
LogMaker lm;

using namespace std;
bool isGameRunning = true;

const int SCREEN_WIDTH = 1040;
const int SCREEN_HEIGHT = 880;

SDL_Rect playerRect;
SDL_Renderer* renderer;
SDL_Texture* playerTexture;
SDL_Window* window;

const float playerSpeed = 200;  // Adjust the value as needed

// Function to load assets
bool loadAssets() {
    SDL_Surface* playerSurface = SDL_LoadBMP("assets/player.bmp");
    if (!playerSurface) {
        cerr << "Failed to load player sprite. SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    return true;
}

#include <vector>

// Define a button structure
struct Button {
    SDL_Rect rect;
    bool isActive = true;
    bool isButtonHovered = false;

    void (*onClick)();
};

std::vector<Button> buttons;
// Create and set up the "Hello" button
Button helloButton;

void sayHelloOnClick() {
    std::cout << "Hello" << std::endl;
}




// Define a struct to represent keybindings
struct Keybinding {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
    SDL_Scancode space;
};

Keybinding keyConfig;
using json = nlohmann::json;

// Set the key map with default keybindings
void useDefaultKeybindings(Keybinding& keyConfig) {
    // Initialize keybindings
    keyConfig.up = SDL_SCANCODE_W;
    keyConfig.down = SDL_SCANCODE_S;
    keyConfig.left = SDL_SCANCODE_A;
    keyConfig.right = SDL_SCANCODE_D;
    keyConfig.space = SDL_SCANCODE_SPACE;
}

// Save custom keybindings to a configuration file
void saveKeybindingsToFile() {
    // Optionally, save keybindings to a JSON file when the game exits
    json keybindings;
    keybindings["up"] = keyConfig.up;
    keybindings["down"] = keyConfig.down;
    keybindings["left"] = keyConfig.left;
    keybindings["right"] = keyConfig.right;
    keybindings["space"] = keyConfig.space;

    std::ofstream configFile("keybindings.json");
    configFile << keybindings.dump(4);
    configFile.close();
}

// Load custom keybindings from a configuration file
void loadKeybindingsFromFile() {
    std::ifstream configFile("keybindings.json");
    if (!configFile.good()) {
        // The file doesn't exist or cannot be read. Provide default keybindings or handle the situation as needed.
        // For now, we'll assume default keybindings.
        useDefaultKeybindings(keyConfig);
        return;
    }

    // The file is open and readable, proceed to load keybindings from it.
    json data;
    configFile >> data;

    // Load keybindings as before
    keyConfig.up = data["up"];
    keyConfig.down = data["down"];
    keyConfig.left = data["left"];
    keyConfig.right = data["right"];
    keyConfig.space = data["space"];

    configFile.close();
}

// Initialize the key map
void InitKeybindings() {
    // Check if the "keybindings.json" file exists
    std::ifstream configFile("keybindings.json");
    if (!configFile.good()) {
        // The file doesn't exist, so create it with default keybindings
        useDefaultKeybindings(keyConfig);
        saveKeybindingsToFile(); // Save the default keybindings to the file
    }
    else {
        // The file exists, so load keybindings from it
        loadKeybindingsFromFile();
    }
}

// ##################################################### ALL TICKS #####################################################
double DeltaTime;

// Handle input using the key configuration
void tickHandleInput() {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[keyConfig.space]) {
        cout << "Space key is pressed" << endl;
    }

    if (keyState[keyConfig.up]) {
        int newY = playerRect.y - lround(playerSpeed * DeltaTime);
        playerRect.y = newY;
    }
    if (keyState[keyConfig.down]) {
        int newY = playerRect.y + lround(playerSpeed * DeltaTime);
        playerRect.y = newY;
    }
    if (keyState[keyConfig.left]) {
        int newX = playerRect.x - lround(playerSpeed * DeltaTime);
        playerRect.x = newX;
    }
    if (keyState[keyConfig.right]) {
        int newX = playerRect.x + lround(playerSpeed * DeltaTime);
        playerRect.x = newX;
    }
}

// Handle game events
void tickHandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isGameRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = { mouseX, mouseY };
                for (Button& button : buttons) {
                    if (SDL_PointInRect(&mousePoint, &button.rect) && button.isActive) {
                        // Button is pressed and active
                        if (button.onClick) {
                            button.onClick();  // Call the button's onClick function
                        }
                        break;
                    }
                }
            }
            break;

        case SDL_MOUSEMOTION:
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            SDL_Point mousePoint = { mouseX, mouseY };
            for (Button& button : buttons) {
                if (SDL_PointInRect(&mousePoint, &button.rect) && button.isActive) {
                    // The button was clicked
                    button.isButtonHovered = true;
                }
                else {
                    button.isButtonHovered = false;
                }
                break;
            }
        }
    }
}

// Render the game frame
void tickRender() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);


    for (Button& button : buttons) {
        // Draw the button
        if (button.isButtonHovered) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green when hovered
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red when not hovered
        }
        SDL_RenderFillRect(renderer, &helloButton.rect);
    }


    // Present the renderer
    SDL_RenderPresent(renderer);
}



// Clean up the game
void cleanUp() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Update the game state
void tickUpdate() {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    
    tickHandleInput();
}

const int desiredFPS = 60;
const int frameDelay = 1000 / desiredFPS; // Time in milliseconds per frame

void TickGameLoop() {
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;

    Uint32 frameStart, frameEnd, frameTime;
    while (isGameRunning) {
        frameStart = SDL_GetTicks();

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        tickHandleInput();
        tickHandleEvents();
        tickUpdate();
        tickRender();

        DeltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
        cout << DeltaTime << std::endl;

        frameEnd = SDL_GetTicks();
        frameTime = frameEnd - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    cleanUp();
}

// Initialize the game
bool init() {
    lm.log(1, "init keybinds...");
    InitKeybindings();

    helloButton.rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32 }; // Set the button's position and size
    helloButton.isActive = true; // Make the button active
    helloButton.onClick = sayHelloOnClick; // Set the onClick function
    // Custom onClick function for the "Hello" button
    buttons.push_back(helloButton);

    lm.log(1, "SDL_INIT_EVERYTHING...");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        isGameRunning = false;
        return false;
    }

    lm.log(1, "SDL_CreateWindow...");
    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
    if (!window) {
        cerr << "Failed to create window. SDL Error: " << SDL_GetError() << std::endl;
        isGameRunning = false;
        return false;
    }

    lm.log(1, "SDL_CreateRendererr...");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Failed to create renderer. SDL Error: " << SDL_GetError() << std::endl;
        isGameRunning = false;
        return false;
    }

    lm.log(1, "Loading assets...");
    if (!loadAssets()) {
        isGameRunning = false;
        return false;
    }

    // Show the window
    SDL_ShowWindow(window);


    playerRect = { 0 , SCREEN_HEIGHT / 2, 32, 32 }; // Assuming a fixed size for the player
    lm.log(0, "Finished Loading! - Ready to play!");
    return true;
}

int main(int argc, char* argv[]) {
    lm.log(1, "Initializing dependencies...");
    if (!init()) {
        cout << "SDL is probably missing";
        cleanUp();
        return 0;
    }
    if (isGameRunning) {
        TickGameLoop();
    }

    saveKeybindingsToFile();

    return 0;
}

