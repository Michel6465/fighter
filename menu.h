#ifndef __MENU_H__
#define __MENU_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

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

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music, SDL_Texture* spaceshipTexture);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect);
int addBullets(SDL_Rect* bullets, SDL_Rect spaceshipRect, int numBullets, int shipLevel);
void initSDL();
SDL_Window* initWindow(const char* title, int width, int height);
SDL_Renderer* initRenderer(SDL_Window* window);
TTF_Font* initFont(const char* path, int size);
void checkInit(int val, char* msg);
void checkInitMix(int val, char* msg);
void checkInitTtf(int val, char* msg);

enum {MAIN_MENU, OPTIONS, GAME};

#endif