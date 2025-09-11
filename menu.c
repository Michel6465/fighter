#include "menu.h"
#include "game.h"

int main(int argc, char* args[]) {
    initSDL();
    SDL_Window* window = initWindow("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = initRenderer(window);
    TTF_Font* font = initFont("sft.ttf", 28);
    TTF_Font* titleFont = initFont("sft.ttf", 48);

    // Title text dimensions
    SDL_Color titleColor = {255, 255, 0};
    SDL_Rect titleRect = { (SCREEN_WIDTH - 400) / 2, 30, 400, 60 };
    
    // Load checkbox images
    SDL_Surface* checkboxCheckedSurface = IMG_Load("img/menus/checkbox_checked.png");
    SDL_Surface* checkboxUncheckedSurface = IMG_Load("img/menus/checkbox_unchecked.png");
    checkInit(!checkboxCheckedSurface || !checkboxUncheckedSurface, "Failed to load checkbox images");

    SDL_Texture* checkboxCheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxCheckedSurface);
    SDL_Texture* checkboxUncheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxUncheckedSurface);
    SDL_FreeSurface(checkboxCheckedSurface);
    SDL_FreeSurface(checkboxUncheckedSurface);
    checkInit(!checkboxCheckedTexture || !checkboxUncheckedTexture, "Failed to create textures from checkbox images");

    SDL_Surface* pauseSurface = IMG_Load("img/menus/pause.png");
    checkInit(!pauseSurface, "Failed to load pause button image");
    SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
    SDL_FreeSurface(pauseSurface);
    checkInit(!pauseTexture, "Failed to create texture from pause button image");

    // Load spaceship image
    SDL_Surface* spaceshipSurface = IMG_Load("img/spaceships/Fighter1-by-MillionthVector/topdownfighter.png");
    checkInit(!spaceshipSurface, "Failed to load spaceship image");
    SDL_Texture* spaceshipTexture = SDL_CreateTextureFromSurface(renderer, spaceshipSurface);
    SDL_FreeSurface(spaceshipSurface);
    checkInit(!spaceshipTexture, "Failed to create texture from spaceship image");
    // Define spaceship rectangle
    SDL_Rect spaceshipRect = { 100, 0, 200, 95 }; // Assuming the spaceship size is 64x64

    // Load bullet image
    SDL_Surface* bulletSurface = IMG_Load("img/bullets/sprites_-_lasers_bullets_1_66v2.5/bullets_full/10.png");
    checkInit(!bulletSurface, "Failed to load bullet image");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    checkInit(!bulletTexture, "Failed to create texture from bullet image");

    // Charger et lire la musique
    Mix_Music* music = Mix_LoadMUS("music/pinball-theme.mp3");
    checkInitMix(music == NULL, "Erreur de chargement de la musique");
    checkInitMix(Mix_PlayMusic(music, -1) == -1, "Erreur lors de la lecture de la musique");

    // Bullet list
    SDL_Rect bullets[MAX_BULLETS];
    int numBullets = 0;

    // Button text dimensions
    SDL_Color buttonTextColor = { 0xFF, 0xFF, 0x00 };
    SDL_Rect startButtonRect = { START_BUTTON_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Rect optionsButtonRect = { OPTIONS_BUTTON_X, OPTIONS_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Rect quitButtonRect = { QUIT_BUTTON_X, QUIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Define checkboxes and back button on the options screen
    SDL_Rect checkbox1Rect = { CHECKBOX_X, CHECKBOX_Y1, CHECKBOX_SIZE, CHECKBOX_SIZE };
    SDL_Rect checkbox2Rect = { CHECKBOX_X, CHECKBOX_Y2, CHECKBOX_SIZE, CHECKBOX_SIZE };
    SDL_Rect backButtonRect = { BACK_BUTTON_X, BACK_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    SDL_Rect pauseButtonRect = { SCREEN_WIDTH - 50 - 10, 10, 50, 50 };
    SDL_Rect scoreRect = { 10, 10, 200, 50 };

    displayData();

    // Main loop flag
    int quit = 0;
    int screen = MAIN_MENU;
    int isSound = 1;
    int isHard = 0;
    int score = 0;
    int shipLevel = 1;
    Uint32 frameStart;
    int frameTime;
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Enregistrer le début de la frame
        frameStart = SDL_GetTicks();

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if the mouse click is within the buttons
                if (screen == MAIN_MENU) {
                    if (x >= startButtonRect.x && x <= (startButtonRect.x + startButtonRect.w) &&
                        y >= startButtonRect.y && y <= (startButtonRect.y + startButtonRect.h)) {
                        printf("Start button clicked!\n");
                        screen = GAME;
                    }
                    else if (x >= optionsButtonRect.x && x <= (optionsButtonRect.x + optionsButtonRect.w) &&
                            y >= optionsButtonRect.y && y <= (optionsButtonRect.y + optionsButtonRect.h)) {
                        printf("Options button clicked!\n");
                        screen = OPTIONS;
                    }
                    else if (x >= quitButtonRect.x && x <= (quitButtonRect.x + quitButtonRect.w) &&
                            y >= quitButtonRect.y && y <= (quitButtonRect.y + quitButtonRect.h)) {
                        printf("Quit button clicked!\n");
                        quit = 1; // Quit the application
                    }
                } else if (screen == OPTIONS) {
                    // Check if checkboxes are clicked
                    if (x >= checkbox1Rect.x && x <= (checkbox1Rect.x + checkbox1Rect.w) &&
                        y >= checkbox1Rect.y && y <= (checkbox1Rect.y + checkbox1Rect.h)) {
                        if (Mix_PlayingMusic()) {
                            if (!isSound) Mix_ResumeMusic();
                            else          Mix_PauseMusic();
                        }
                        isSound = !isSound;
                    }
                    if (x >= checkbox2Rect.x && x <= (checkbox2Rect.x + checkbox2Rect.w) &&
                        y >= checkbox2Rect.y && y <= (checkbox2Rect.y + checkbox2Rect.h)) {
                        isHard = !isHard;
                    }
                    // Check if the back button is clicked
                    if (x >= backButtonRect.x && x <= (backButtonRect.x + backButtonRect.w) &&
                        y >= backButtonRect.y && y <= (backButtonRect.y + backButtonRect.h)) {
                        printf("Back button clicked!\n");
                        screen = MAIN_MENU;
                    }
                } else if (screen == GAME) {
                    // Check if the pause button is clicked
                    if (x >= pauseButtonRect.x && x <= (pauseButtonRect.x + pauseButtonRect.w) &&
                        y >= pauseButtonRect.y && y <= (pauseButtonRect.y + pauseButtonRect.h)) {
                        printf("Pause button clicked!\n");
                        screen = MAIN_MENU;
                    }

                    numBullets = addBullets(bullets, spaceshipRect, numBullets, shipLevel);
                }
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_p:
                        if (screen == GAME) {
                            printf("P key pressed - going back to main menu!\n");
                            screen = MAIN_MENU;
                        }
                        break;
                }
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        if (screen == MAIN_MENU) {
            // Render title
            renderText(renderer, titleFont, "Anticonstitutionnellement", titleColor, &titleRect);

            // Render buttons
            SDL_SetRenderDrawColor(renderer, 0x66, 0x00, 0x66, 0xFF);
            SDL_RenderFillRect(renderer, &startButtonRect);
            SDL_RenderFillRect(renderer, &optionsButtonRect);
            SDL_RenderFillRect(renderer, &quitButtonRect);

            // Render button borders
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black border
            SDL_RenderDrawRect(renderer, &startButtonRect);
            SDL_RenderDrawRect(renderer, &optionsButtonRect);
            SDL_RenderDrawRect(renderer, &quitButtonRect);

            // Render button texts
            SDL_Rect startTextRect = { START_BUTTON_X + 10, START_BUTTON_Y + 10, BUTTON_WIDTH - 20, BUTTON_HEIGHT - 20 };
            SDL_Rect optionsTextRect = { OPTIONS_BUTTON_X + 10, OPTIONS_BUTTON_Y + 10, BUTTON_WIDTH - 20, BUTTON_HEIGHT - 20 };
            SDL_Rect quitTextRect = { QUIT_BUTTON_X + 10, QUIT_BUTTON_Y + 10, BUTTON_WIDTH - 20, BUTTON_HEIGHT - 20 };
            renderText(renderer, font, "Start", buttonTextColor, &startTextRect);
            renderText(renderer, font, "Options", buttonTextColor, &optionsTextRect);
            renderText(renderer, font, "Quit", buttonTextColor, &quitTextRect);
        } else if (screen == OPTIONS) {
            // Render title
            renderText(renderer, titleFont, "Options", titleColor, &titleRect);

            if (isSound) SDL_RenderCopy(renderer, checkboxCheckedTexture, NULL, &checkbox1Rect);
            else SDL_RenderCopy(renderer, checkboxUncheckedTexture, NULL, &checkbox1Rect);

            if (isHard) SDL_RenderCopy(renderer, checkboxCheckedTexture, NULL, &checkbox2Rect);
            else SDL_RenderCopy(renderer, checkboxUncheckedTexture, NULL, &checkbox2Rect);

            // Render back button
            SDL_SetRenderDrawColor(renderer, 0x66, 0x00, 0x66, 0xFF); // Same color as other buttons
            SDL_RenderFillRect(renderer, &backButtonRect);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black border
            SDL_RenderDrawRect(renderer, &backButtonRect);

            // Render texts
            SDL_Rect checkbox1TextRect = { checkbox1Rect.x + CHECKBOX_SIZE + 10, checkbox1Rect.y, 200, CHECKBOX_SIZE };
            SDL_Rect checkbox2TextRect = { checkbox2Rect.x + CHECKBOX_SIZE + 10, checkbox2Rect.y, 200, CHECKBOX_SIZE };
            SDL_Rect backTextRect = { BACK_BUTTON_X + 10, BACK_BUTTON_Y + 10, BUTTON_WIDTH - 20, BUTTON_HEIGHT - 20 };
            renderText(renderer, font, "Sound", buttonTextColor, &checkbox1TextRect);
            renderText(renderer, font, "Hardcore", buttonTextColor, &checkbox2TextRect);
            renderText(renderer, font, "Back", buttonTextColor, &backTextRect);
        } else if (screen == GAME) {
            // Move bullets
            for (int i = 0; i < numBullets; i++) {
                bullets[i].x += 7; // Move bullet 1 pixel to the right

                // Remove bullets that move off-screen
                if (bullets[i].x > SCREEN_WIDTH) {
                    for (int j = i; j < numBullets - 1; j++) {
                        bullets[j] = bullets[j + 1];
                    }
                    numBullets--;
                    i--; // Recheck the current index
                }
            }

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Update spaceship vertical position based on mouse Y position
            spaceshipRect.y = mouseY - (spaceshipRect.h / 2);

            // Keep spaceship within window bounds
            if (spaceshipRect.y < 0) spaceshipRect.y = 0;
            if (spaceshipRect.y + spaceshipRect.h > SCREEN_HEIGHT) spaceshipRect.y = SCREEN_HEIGHT - spaceshipRect.h;

            // Render score
            char scoreText[20];
            sprintf(scoreText, "Score: %d", score);
            renderText(renderer, font, scoreText, titleColor, &scoreRect);

            // Pause and spaceship
            SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseButtonRect);
            SDL_RenderCopy(renderer, spaceshipTexture, NULL, &spaceshipRect);

            // Render bullets
            for (int i = 0; i < numBullets; i++) {
                SDL_RenderCopy(renderer, bulletTexture, NULL, &bullets[i]);
            }

            score++;
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Limiter les FPS
        frameTime = SDL_GetTicks() - frameStart;

        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // Clean up resources
    cleanUp(window, renderer, font, music, spaceshipTexture);

    return 0;
}


void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    checkInit(textSurface == NULL, "Unable to render text surface");
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    checkInit(textTexture == NULL, "Unable to create texture from rendered text");

    SDL_RenderCopy(renderer, textTexture, NULL, dstRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

SDL_Window* initWindow(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    checkInit(window == NULL, "Window could not be created");
    return window;
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    checkInit(renderer == NULL, "Renderer could not be created");
    return renderer;
}

TTF_Font* initFont(const char* path, int size) {
    TTF_Font* font = TTF_OpenFont(path, size);
    checkInit(font == NULL, "Failed to load font");
    return font;
}

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music, SDL_Texture* spaceshipTexture) {
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyTexture(spaceshipTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void initSDL() {
    checkInit(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0, "SDL could not initialize");
    checkInitTtf(TTF_Init() == -1, "SDL_ttf could not initialize");
    checkInitMix(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0, "Erreur d'initialisation de SDL_mixer");
}

void checkInit(int val, char* msg) {
    if (val) {
        printf("%s!\nSDL_Error: %s\n", msg, SDL_GetError());
        exit(1);
    }
}

// Pas implémenté : free_music pour le cas où il n'arriverait pas à la jouer
void checkInitMix(int val, char* msg) {
    if (val) {
        printf("%s!\nMix_Error: %s\n", msg, Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        exit(1);
    }
}

void checkInitTtf(int val, char* msg) {
    if (val) {
        printf("%s!\nMix_Error: %s\n", msg, TTF_GetError());
        SDL_Quit();
        exit(1);
    }
}