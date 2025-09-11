#ifndef __MENU_H__
#define __MENU_H__

#include <SDL2/SDL.h>
#include "init.h"  // Needs GameResources and UIElements

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music, SDL_Texture* spaceshipTexture);

#endif