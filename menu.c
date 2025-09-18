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

void handleMouseInput(Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, SDL_Event e, int* quit) {
    int x, y;

    // User requests quit
    if (e.type == SDL_QUIT) {
        *quit = 1;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);

        // Check if the mouse click is within the buttons
        if (game->screen == MAIN_MENU) {
            int lineHeight = 80;
            int currentYPosition = resources->windowHeight - lineHeight - 20;
            for (int i=ui->nbMenuButtons-1; i>=0; i--){
                if (x >= MENU_MARGIN_RIGHT && x <= (MENU_MARGIN_RIGHT + ui->menuButtons[i].w) &&
                    y >= currentYPosition && y <= (currentYPosition + ui->menuButtons[i].h)) {
                    printf("%s button clicked!\n", ui->menuButtons[i].text);

                    if (i==0) game->screen = GAME;
                    else if (i==1) game->screen = OPTIONS;
                    else if (i==2) *quit = 1;

                }
                currentYPosition -= lineHeight;
            }
        } else if (game->screen == OPTIONS) {
            int lineHeight = 80;
            int currentYPosition = resources->windowHeight - lineHeight - 20;
            for (int i=ui->nbOptionsButtons-1; i>=0; i--){
                if (ui->optionsButtons[i].isHovering) {
                    if (i==0) {
                        if (x >= MENU_MARGIN_RIGHT + MENU_OFFSET && x < MENU_MARGIN_RIGHT + MENU_OFFSET + ui->optionsButtons[0].slider.length) {
                            float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[0].slider.length, 0.0f), 1.0f);
                            setMusicVolume(resources, newVolume);
                            
                            // Update knob position
                            ui->optionsButtons[0].slider.knobPosition = newVolume;
                            ui->draggingMusic = 1;
                        }
                    } else if (i==1) {
                        if (x >= MENU_MARGIN_RIGHT + MENU_OFFSET && x < MENU_MARGIN_RIGHT + MENU_OFFSET + ui->optionsButtons[1].slider.length) {
                            float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[1].slider.length, 0.0f), 1.0f);
                            setSoundEffectsVolume(resources, newVolume);
                            
                            // Update knob position
                            ui->optionsButtons[1].slider.knobPosition = newVolume;
                            ui->draggingSfx = 1;
                        }
                    } else if (i==2) {
                        if (x >= MENU_MARGIN_RIGHT && x <= MENU_MARGIN_RIGHT + MENU_OFFSET + min(ui->optionsButtons[i].checkbox.boxSize, ui->optionsButtons[i].h))
                            ui->optionsButtons[i].checkbox.isChecked = !ui->optionsButtons[i].checkbox.isChecked;
                    } else if (i==3) {
                        game->screen = MAIN_MENU;
                    }
                }
                currentYPosition -= lineHeight;
            }
        } else if (game->screen == GAME) {
            // Check if the pause button is clicked
            if (x >= ui->pauseButtonRect.x && x <= (ui->pauseButtonRect.x + ui->pauseButtonRect.w) &&
                y >= ui->pauseButtonRect.y && y <= (ui->pauseButtonRect.y + ui->pauseButtonRect.h)) {
                printf("Pause button clicked!\n");
                game->screen = MAIN_MENU;
            } else {
                //numBullets = addBullets(bullets, fighter.rect, numBullets, shipLevel);
            }
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (game->screen == OPTIONS) {
            SDL_GetMouseState(&x, &y);
            if (ui->draggingMusic) {
                float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[0].slider.length, 0.0f), 1.0f);
                setMusicVolume(resources, newVolume);
                
                // Update knob position
                ui->optionsButtons[0].slider.knobPosition = newVolume;
                ui->draggingMusic = 0;
                ui->optionsButtons[0].isHovering = 0;
            }

            if (ui->draggingSfx) {
                float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[1].slider.length, 0.0f), 1.0f);
                setSoundEffectsVolume(resources, newVolume);
                
                // Update knob position
                ui->optionsButtons[1].slider.knobPosition = newVolume;
                ui->draggingSfx = 0;
                ui->optionsButtons[1].isHovering = 0;
            }
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        if (game->screen == OPTIONS) {
            if (ui->draggingMusic) {
                float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[0].slider.length, 0.0f), 1.0f);
                setMusicVolume(resources, newVolume);
                ui->optionsButtons[0].slider.knobPosition = newVolume;
            }
            
            if (ui->draggingSfx) {
                float newVolume = min(max((float)(x - MENU_MARGIN_RIGHT-MENU_OFFSET) / ui->optionsButtons[1].slider.length, 0.0f), 1.0f);
                setSoundEffectsVolume(resources, newVolume);
                ui->optionsButtons[1].slider.knobPosition = newVolume;
            }
        }
        
        if (game->screen == MAIN_MENU) {
            int lineHeight = 80;
            int currentYPosition = resources->windowHeight - lineHeight - 20;
            for (int i=ui->nbMenuButtons-1; i>=0; i--){
                if (x >= MENU_MARGIN_RIGHT && x <= (MENU_MARGIN_RIGHT + ui->menuButtons[i].w) &&
                    y >= currentYPosition && y <= (currentYPosition + ui->menuButtons[i].h)) {
                    ui->menuButtons[i].isHovering = 1;
                } else {
                    ui->menuButtons[i].isHovering = 0;
                }
                currentYPosition -= lineHeight;
            }
        } else if (game->screen == OPTIONS) {
            int lineHeight = 80;
            int currentYPosition = resources->windowHeight - lineHeight - 20;
            for (int i=ui->nbOptionsButtons-1; i>=0; i--){
                if (x >= MENU_MARGIN_RIGHT && x <= (MENU_MARGIN_RIGHT + ui->optionsButtons[i].w) &&
                y >= currentYPosition && y <= (currentYPosition + ui->optionsButtons[i].h)
                && (!(i!=0 && ui->draggingMusic) && !(i!=1 && ui->draggingSfx))) { // if 0 nor 1 dragging
                    ui->optionsButtons[i].isHovering = 1;
                } else {
                    if ((i==0 && !ui->draggingMusic) || (i==1 && !ui->draggingSfx) || i>1)
                        ui->optionsButtons[i].isHovering = 0;
                }
                currentYPosition -= lineHeight;
            }
        } else if (game->screen == GAME) {
            // Check if the pause button is clicked
            if (x >= ui->pauseButtonRect.x && x <= (ui->pauseButtonRect.x + ui->pauseButtonRect.w) &&
            y >= ui->pauseButtonRect.y && y <= (ui->pauseButtonRect.y + ui->pauseButtonRect.h)) {
                resources->isHoveringPause = 1;
            } else {
                resources->isHoveringPause = 0;
                //numBullets = addBullets(bullets, fighter.rect, numBullets, shipLevel);
            }
        }
    }
}

void handleKeyboardInput(Game* game, Fighter* fighter, GameResources* resources, int* quit) {
    // Toggle fullscreen with F11 key
    if (game->keyState[SDL_SCANCODE_F]) {
        static int is_fullscreen = 0;
        is_fullscreen = !is_fullscreen;
        
        if (is_fullscreen) {
            SDL_SetWindowFullscreen(resources->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            SDL_SetWindowFullscreen(resources->window, 0);
        }
        
        // Add a small delay to prevent multiple toggles
        SDL_Delay(200);
    }

    if (game->screen == GAME) {
        int is_thrusting = 0;

        // Check for P key (pause) - use key press for one-time actions
        if (game->keyState[SDL_SCANCODE_P]) {
            printf("P key pressed - going back to main menu!\n");
            game->screen = MAIN_MENU;
            // Add a small delay to prevent multiple triggers
            SDL_Delay(200);
        }

        // Handle continuous movement keys
        if (game->keyState[SDL_SCANCODE_UP]) {
            is_thrusting = 1;
            float rad_angle = fighter->angle * M_PI / 180.0f;
            fighter->speed_x += sin(rad_angle) * FIGHTER_SPEED;
            fighter->speed_y += -cos(rad_angle) * FIGHTER_SPEED;
        }
        
        if (game->keyState[SDL_SCANCODE_DOWN]) {
            int action = getShortestRotationDirection(fighter);
            if (action == THRUST) { // accelerate if angle opposite to speed
                is_thrusting = 1;
                float rad_angle = fighter->angle * M_PI / 180.0f;
                fighter->speed_x += sin(rad_angle) * FIGHTER_SPEED;
                fighter->speed_y += -cos(rad_angle) * FIGHTER_SPEED;
            } else {
                switch(action) {
                    case TURN_LEFT:
                        fighter->angle -= ANGLES_PER_FRAME;
                        break;
                    case TURN_RIGHT:
                        fighter->angle += ANGLES_PER_FRAME;
                        break;
                    default: // DO_NOTHING
                        break;
                }
            }
        }
        
        if (game->keyState[SDL_SCANCODE_LEFT]) {
            fighter->angle -= ANGLES_PER_FRAME;
        }
        
        if (game->keyState[SDL_SCANCODE_RIGHT]) {
            fighter->angle += ANGLES_PER_FRAME;
        }
        
        if (game->keyState[SDL_SCANCODE_SPACE]) {
            fighter->speed_x = 0;
            fighter->speed_y = 0;
            fighter->angle = 0;
            resources->bg_x = 0;
            resources->bg_y = 0;
        }

        // Update thruster animation
        updateThruster(&fighter->thruster, is_thrusting);
    } else if (game->screen == MAIN_MENU) {
        if (game->keyState[SDL_SCANCODE_Q]) { // A
            game->screen = GAME;
        }
    }

    if (game->keyState[SDL_SCANCODE_A]) { // Q on AZERTY keyboard
        *quit = 1;
    }
}

// void updateSound(GameResources* resources, UIElements* ui, int x, int i) {

// }