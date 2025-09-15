#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "init.h"

#define ANGLES_PER_FRAME 5
#define SPEED_MULTIPLICATOR 0.00008

enum {TURN_LEFT, TURN_RIGHT, THRUST, DO_NOTHING};

// Function declarations
void handleMouseInput(Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, SDL_Event e, int* quit);
void handleKeyboardInput(Game* game, Fighter* fighter, GameResources* resources, int* quit);
void updateGameState(Game* game, Fighter* fighter, GameResources* resources, BackgroundEffects* bg_effects);
//int addBullets(SDL_Rect* bullets, SDL_Rect spaceshipRect, int numBullets, int shipLevel);
void updateSolarSystem(BackgroundEffects* bg_effects);
void updateThruster(ThrusterState* thruster, int is_thrusting);

int getShortestRotationDirection(Fighter* fighter);
float getFighterMovementDirection(Fighter* fighter);
float getFighterMovementSpeed(Fighter* fighter);
void limitFighterSpeed(Fighter* fighter, float max_speed);
void calculateGravityForces(Fighter* fighter, BackgroundEffects* bg_effects, GameResources* resources);
int checkAstralObjectDiscovery(Fighter* fighter, BackgroundEffects* bg_effects, GameResources* resources, Game* game);

#endif