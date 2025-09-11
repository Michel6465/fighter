#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "init.h"

#define ANGLES_PER_FRAME 5

enum {TURN_LEFT, TURN_RIGHT, THRUST, DO_NOTHING};

// Game states
typedef enum {
    MAIN_MENU,
    GAME,
    OPTIONS
} GameState;

// Game structure
typedef struct {
    GameState screen;
    int isSound;
    int isHard;
    int score;
    int shipLevel;
    int numBullets;
    SDL_Rect bullets[MAX_BULLETS];
    const Uint8* keyState;
} Game;

// Function declarations
void handleKeyboardInput(Game* game, Fighter* fighter, GameResources* resources);
void updateGameState(Game* game, Fighter* fighter, GameResources* resources, UIElements* ui);
int addBullets(SDL_Rect* bullets, SDL_Rect spaceshipRect, int numBullets, int shipLevel);
int getShortestRotationDirection(Fighter* fighter);
float getFighterMovementDirection(Fighter* fighter);
float getFighterMovementSpeed(Fighter* fighter);

#endif