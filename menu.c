#include "menu.h"
#include "game.h"   // Needs Game struct
#include "init.h"   // Needs resources and UI elements
#include "render.h" // Render menu
#include "sounds.h"
#include <stdio.h>

int main() {
    // Initialize variables
    Fighter fighter;
    GameResources resources;
    UIElements ui;
    Game game;

    SDL_Rect bullets[MAX_BULLETS];
    
    // Initialize SDL systems
    initSDLSystems();
    
    // Create window and renderer
    initWindow("Fighter game", &resources);
    SDL_Renderer* renderer = initRenderer(resources.window);
    
    // Initialize game components
    initGameResources(renderer, &resources);
    initUIElements(&ui, resources.window);
    initGame(&game);
    initFighter(&fighter, resources.windowWidth, resources.windowHeight);

    BackgroundEffects bg_effects;
    generateStarfield(&bg_effects);
    initSolarSystem(&bg_effects);
    initAstralObjects(&bg_effects, &resources);
    initDiscoverySystem(&game);

    SDL_SetWindowFullscreen(resources.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    // Main loop flag
    int quit = 0;
    Uint32 frameStart;
    int frameTime;
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Enregistrer le début de la frame
        frameStart = SDL_GetTicks();

        // Handle events on queue (only for non-keyboard events)
        while (SDL_PollEvent(&e) != 0) {
            handleMouseInput(&game, &fighter, &resources, &ui, e, &quit);
        }

        // Update keyboard state
        SDL_PumpEvents();

        // Handle keyboard input
        handleKeyboardInput(&game, &fighter, &resources, &quit);

        // Update game state
        updateGameState(&game, &fighter, &resources, &bg_effects);

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
    if (resources.window) SDL_DestroyWindow(resources.window);

    return 0;
}
