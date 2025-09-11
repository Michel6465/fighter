#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Cap FPS
#define FPS 120
#define FRAME_DELAY 1000/FPS // Durée d'une frame en millisecondes (16ms pour 60 FPS)

// Screen dimension constants
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

// Button dimensions and positions
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_SPACING 20

// Define positions for the options screen UI elements
#define CHECKBOX_X 150
#define CHECKBOX_Y1 150
#define CHECKBOX_Y2 200
#define CHECKBOX_SIZE 50

#define BACK_BUTTON_X (SCREEN_WIDTH - BUTTON_WIDTH) / 2
#define BACK_BUTTON_Y 300

#define START_BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)
#define START_BUTTON_Y (SCREEN_HEIGHT / 2 - (BUTTON_HEIGHT + BUTTON_SPACING) * 1.5)
#define OPTIONS_BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)
#define OPTIONS_BUTTON_Y (SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2)
#define QUIT_BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)
#define QUIT_BUTTON_Y (SCREEN_HEIGHT / 2 + (BUTTON_HEIGHT + BUTTON_SPACING) / 2)

#define FIGHTER_WIDTH 100
#define FIGHTER_HEIGHT 200
#define FIGHTER_SPEED 0.1
#define FIGHTER_MAX_SPEED 0.5
#define MAX_BULLETS 1000
#define PI 3.14159265f

typedef struct {
    SDL_Texture* texture;
    int x, y;
    float speed_x, speed_y;
    float angle;
    SDL_Rect rect;
} Fighter;

typedef struct {
    SDL_Texture* fighterTexture;
    SDL_Texture* lunaTexture;
    SDL_Texture* pauseTexture;
    SDL_Texture* checkboxCheckedTexture;
    SDL_Texture* checkboxUncheckedTexture;
    SDL_Texture* bulletTexture;
    Mix_Music* music;
    TTF_Font* font;
    TTF_Font* titleFont;
    float bg_x, bg_y;
} GameResources;

typedef struct {
    SDL_Rect startButtonRect;
    SDL_Rect optionsButtonRect;
    SDL_Rect quitButtonRect;
    SDL_Rect checkbox1Rect;
    SDL_Rect checkbox2Rect;
    SDL_Rect backButtonRect;
    SDL_Rect pauseButtonRect;
    SDL_Rect scoreRect;
    SDL_Rect titleRect;
    SDL_Color titleColor;
    SDL_Color buttonTextColor;
} UIElements;

// Function declarations
void checkInit(int val, const char* msg);
void checkInitMix(int condition, const char* message);

void initSDLSystems();
SDL_Window* initWindow(const char* title, int width, int height);
SDL_Renderer* initRenderer(SDL_Window* window);
TTF_Font* initFont(const char* fontPath, int size);
void initGameResources(SDL_Renderer* renderer, GameResources* resources);
void initUIElements(UIElements* ui);
void initFighter(Fighter* fighter);
void cleanupResources(GameResources* resources);

#endif