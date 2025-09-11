#include "init.h"
#include <stdio.h>

// Helper function for error checking
void checkInit(int condition, const char* message) {
    if (condition) {
        printf("Error: %s\n", message);
        exit(1);
    }
}

void checkInitMix(int condition, const char* message) {
    if (condition) {
        printf("Mix Error: %s\n", message);
        exit(1);
    }
}

void initSDLSystems() {
    // Initialize SDL
    checkInit(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0, "SDL could not initialize!");
    
    // Initialize SDL_ttf
    checkInit(TTF_Init() == -1, "SDL_ttf could not initialize!");
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    checkInit(!(IMG_Init(imgFlags) & imgFlags), "SDL_image could not initialize!");
    
    // Initialize SDL_mixer
    checkInitMix(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0, "SDL_mixer could not initialize!");
}

SDL_Window* initWindow(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(title,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        width,
                                        height,
                                        SDL_WINDOW_SHOWN);
    checkInit(!window, "Window could not be created!");
    return window;
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    checkInit(!renderer, "Renderer could not be created!");
    return renderer;
}

TTF_Font* initFont(const char* fontPath, int size) {
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    checkInit(!font, "Failed to load font!");
    return font;
}

void initGameResources(SDL_Renderer* renderer, GameResources* resources) {
    // Load checkbox images
    SDL_Surface* checkboxCheckedSurface = IMG_Load("img/menus/checkbox_checked.png");
    SDL_Surface* checkboxUncheckedSurface = IMG_Load("img/menus/checkbox_unchecked.png");
    checkInit(!checkboxCheckedSurface || !checkboxUncheckedSurface, "Failed to load checkbox images");

    resources->checkboxCheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxCheckedSurface);
    resources->checkboxUncheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxUncheckedSurface);
    SDL_FreeSurface(checkboxCheckedSurface);
    SDL_FreeSurface(checkboxUncheckedSurface);
    checkInit(!resources->checkboxCheckedTexture || !resources->checkboxUncheckedTexture, "Failed to create checkbox textures");

    // Load luna image
    SDL_Surface* lunaSurface = IMG_Load("img/luna.png");
    checkInit(!lunaSurface, "Failed to load luna image");
    resources->lunaTexture = SDL_CreateTextureFromSurface(renderer, lunaSurface);
    SDL_FreeSurface(lunaSurface);
    checkInit(!resources->lunaTexture, "Failed to create luna texture");

    // Load fighter image
    SDL_Surface* fighterSurface = IMG_Load("img/topdownfighter.png");
    checkInit(!fighterSurface, "Failed to load fighter image");
    resources->fighterTexture = SDL_CreateTextureFromSurface(renderer, fighterSurface);
    SDL_FreeSurface(fighterSurface);
    checkInit(!resources->fighterTexture, "Failed to create fighter texture");

    // Load pause button
    SDL_Surface* pauseSurface = IMG_Load("img/menus/pause.png");
    checkInit(!pauseSurface, "Failed to load pause button image");
    resources->pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
    SDL_FreeSurface(pauseSurface);
    checkInit(!resources->pauseTexture, "Failed to create pause texture");

    // Load bullet image
    SDL_Surface* bulletSurface = IMG_Load("img/bullets/sprites_-_lasers_bullets_1_66v2.5/bullets_full/10.png");
    checkInit(!bulletSurface, "Failed to load bullet image");
    resources->bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    checkInit(!resources->bulletTexture, "Failed to create bullet texture");

    // Load music
    resources->music = Mix_LoadMUS("music/pinball-theme.mp3");
    checkInitMix(resources->music == NULL, "Failed to load music");
    checkInitMix(Mix_PlayMusic(resources->music, -1) == -1, "Failed to play music");

    // Initialize fonts
    resources->font = initFont("sft.ttf", 28);
    resources->titleFont = initFont("sft.ttf", 48);

    // Initialize background position
    resources->bg_x = 0;
    resources->bg_y = 0;

    printf("%d %d %d\n", FIGHTER_WIDTH, FIGHTER_HEIGHT, FIGHTER_SPEED);
}

void initUIElements(UIElements* ui) {
    // Title text dimensions
    ui->titleRect = (SDL_Rect){ (SCREEN_WIDTH - 400) / 2, 30, 400, 60 };
    
    // Button rectangles
    ui->startButtonRect = (SDL_Rect){ START_BUTTON_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->optionsButtonRect = (SDL_Rect){ OPTIONS_BUTTON_X, OPTIONS_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->quitButtonRect = (SDL_Rect){ QUIT_BUTTON_X, QUIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    
    // Options screen elements
    ui->checkbox1Rect = (SDL_Rect){ CHECKBOX_X, CHECKBOX_Y1, CHECKBOX_SIZE, CHECKBOX_SIZE };
    ui->checkbox2Rect = (SDL_Rect){ CHECKBOX_X, CHECKBOX_Y2, CHECKBOX_SIZE, CHECKBOX_SIZE };
    ui->backButtonRect = (SDL_Rect){ BACK_BUTTON_X, BACK_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    
    // Game screen elements
    ui->pauseButtonRect = (SDL_Rect){ SCREEN_WIDTH - 50 - 10, 10, 50, 50 };
    ui->scoreRect = (SDL_Rect){ 10, 10, 200, 50 };

    ui->titleColor = (SDL_Color){ 255, 255, 0, 0 };
    ui->buttonTextColor = (SDL_Color){ 255, 255, 0, 0 };
}

void initFighter(Fighter* fighter) {
    // Load spaceship image (texture should be loaded separately)
    fighter->speed_x = 0;
    fighter->speed_y = 0;
    fighter->x = SCREEN_WIDTH / 2 - FIGHTER_WIDTH / 2;
    fighter->y = SCREEN_HEIGHT / 2 - FIGHTER_HEIGHT / 2;
    fighter->angle = 0;
    fighter->rect = (SDL_Rect){ fighter->x, fighter->y, FIGHTER_WIDTH, FIGHTER_HEIGHT };
}

void cleanupResources(GameResources* resources) {
    if (resources->lunaTexture) SDL_DestroyTexture(resources->lunaTexture);
    if (resources->pauseTexture) SDL_DestroyTexture(resources->pauseTexture);
    if (resources->checkboxCheckedTexture) SDL_DestroyTexture(resources->checkboxCheckedTexture);
    if (resources->checkboxUncheckedTexture) SDL_DestroyTexture(resources->checkboxUncheckedTexture);
    if (resources->bulletTexture) SDL_DestroyTexture(resources->bulletTexture);
    if (resources->music) Mix_FreeMusic(resources->music);
    if (resources->font) TTF_CloseFont(resources->font);
    //if (resources->titleFont) TTF_CloseFont(resources->titleFont); // segfault maybe because the first closefont closes all the sizes at the same time
    
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}