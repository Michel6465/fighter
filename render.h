#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "init.h"  // Needs GameResources and UIElements
#include "game.h"       // Needs Game and Fighter

void renderGameScreen(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects);
void renderMainMenu(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui);
void renderOptionsScreen(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui);
void renderGameplay(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects);

void renderThruster(SDL_Renderer* renderer, Fighter* fighter, GameResources* resources);
void renderSingleThruster(SDL_Renderer* renderer, SDL_Texture* texture, Fighter* fighter, SDL_Point offset);

void renderOrbitalTrails(SDL_Renderer* renderer, BackgroundEffects* bg_effects, float camera_x, float camera_y);
void renderSolarSystem(SDL_Renderer* renderer, BackgroundEffects* bg_effects, 
    GameResources* resources, float camera_x, float camera_y);
void renderStarfield(SDL_Renderer* renderer, BackgroundEffects* bg_effects, 
    GameResources* resources, float camera_x, float camera_y);

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect);
        
#endif