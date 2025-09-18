#ifndef __MENU_H__
#define __MENU_H__

#include <SDL2/SDL.h>
#include "init.h"  // Needs GameResources and UIElements

void handleMouseInput(Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, SDL_Event e, int* quit);
void handleKeyboardInput(Game* game, Fighter* fighter, GameResources* resources, int* quit);

#endif