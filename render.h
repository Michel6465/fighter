#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "init.h"  // Needs GameResources and UIElements
#include "game.h"       // Needs Game and Fighter

#define LUNA_SIZE 4000

void renderGameScreen(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui);
void renderMainMenu(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui);
void renderOptionsScreen(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui);
void renderGameplay(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect);

#endif