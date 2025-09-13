#include "menu.h"
#include "game.h"   // Needs Game struct
#include "init.h"   // Needs resources and UI elements
#include "render.h" // Render menu
#include <stdio.h>

int main(int argc, char* args[]) {
    // Initialize variables
    Fighter fighter;
    GameResources resources;
    UIElements ui;

    SDL_Rect bullets[MAX_BULLETS];
    
    // Initialize SDL systems
    initSDLSystems();
    
    // Create window and renderer
    SDL_Window* window = initWindow("Fighter", SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = initRenderer(window);
    
    // Initialize game components
    initGameResources(renderer, &resources);
    initUIElements(&ui);
    initFighter(&fighter);

    Game game = {
        .screen = MAIN_MENU,
        .isSound = 1,
        .isHard = 0,
        .score = 0,
        .shipLevel = 1,
        .numBullets = 0,
        .keyState = SDL_GetKeyboardState(NULL)
    };

    BackgroundEffects bg_effects;
    generateStarfield(&bg_effects);
    initSolarSystem(&bg_effects);
    
    // Main loop flag
    int quit = 0;
    Uint32 frameStart;
    int frameTime;
    int deltaTime;
    SDL_Event e;

    // Get keyboard state
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    // While application is running
    while (!quit) {
        // Enregistrer le début de la frame
        frameStart = SDL_GetTicks();

        // Handle events on queue (only for non-keyboard events)
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if the mouse click is within the buttons
                if (game.screen == MAIN_MENU) {
                    if (x >= ui.startButtonRect.x && x <= (ui.startButtonRect.x + ui.startButtonRect.w) &&
                        y >= ui.startButtonRect.y && y <= (ui.startButtonRect.y + ui.startButtonRect.h)) {
                        printf("Start button clicked!\n");
                        game.screen = GAME;
                    }
                    else if (x >= ui.optionsButtonRect.x && x <= (ui.optionsButtonRect.x + ui.optionsButtonRect.w) &&
                            y >= ui.optionsButtonRect.y && y <= (ui.optionsButtonRect.y + ui.optionsButtonRect.h)) {
                        printf("Options button clicked!\n");
                        game.screen = OPTIONS;
                    }
                    else if (x >= ui.quitButtonRect.x && x <= (ui.quitButtonRect.x + ui.quitButtonRect.w) &&
                            y >= ui.quitButtonRect.y && y <= (ui.quitButtonRect.y + ui.quitButtonRect.h)) {
                        printf("Quit button clicked!\n");
                        quit = 1; // Quit the application
                    }
                } else if (game.screen == OPTIONS) {
                    // Check if checkboxes are clicked
                    if (x >= ui.checkbox1Rect.x && x <= (ui.checkbox1Rect.x + ui.checkbox1Rect.w) &&
                        y >= ui.checkbox1Rect.y && y <= (ui.checkbox1Rect.y + ui.checkbox1Rect.h)) {
                        if (Mix_PlayingMusic()) {
                            if (!game.isSound) Mix_ResumeMusic();
                            else               Mix_PauseMusic();
                        }
                        game.isSound = !game.isSound;
                    }
                    if (x >= ui.checkbox2Rect.x && x <= (ui.checkbox2Rect.x + ui.checkbox2Rect.w) &&
                        y >= ui.checkbox2Rect.y && y <= (ui.checkbox2Rect.y + ui.checkbox2Rect.h)) {
                        game.isHard = !game.isHard;
                    }
                    // Check if the back button is clicked
                    if (x >= ui.backButtonRect.x && x <= (ui.backButtonRect.x + ui.backButtonRect.w) &&
                        y >= ui.backButtonRect.y && y <= (ui.backButtonRect.y + ui.backButtonRect.h)) {
                        printf("Back button clicked!\n");
                        game.screen = MAIN_MENU;
                    }
                } else if (game.screen == GAME) {
                    // Check if the pause button is clicked
                    if (x >= ui.pauseButtonRect.x && x <= (ui.pauseButtonRect.x + ui.pauseButtonRect.w) &&
                        y >= ui.pauseButtonRect.y && y <= (ui.pauseButtonRect.y + ui.pauseButtonRect.h)) {
                        printf("Pause button clicked!\n");
                        game.screen = MAIN_MENU;
                    } else {
                        //numBullets = addBullets(bullets, fighter.rect, numBullets, shipLevel);
                    }
                }
            }
        }

        // Update keyboard state
        SDL_PumpEvents();

        // Handle keyboard input
        handleKeyboardInput(&game, &fighter, &resources, &quit);

        // Update game state
        updateGameState(&game, &fighter, &resources, &ui, &bg_effects);

        // Render game
        renderGameScreen(renderer, &game, &fighter, &resources, &ui, &bg_effects);

        // Frame rate limiting
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // Cleanup
    if (fighter.texture) SDL_DestroyTexture(fighter.texture);
    cleanupResources(&resources);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    return 0;
}
