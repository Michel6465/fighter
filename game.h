#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define MAX_BULLETS 1000
#define BULLET_SIZE 50
#define MAX_SHIPS 100
#define MAX_VERTICES 100
#define MAX_LINE_LENGTH 256

typedef struct Vertex Vertex;
typedef struct FiringPattern FiringPattern;
typedef struct PolyHitbox PolyHitbox;
typedef struct CircleHitbox CircleHitbox;
typedef struct Bullet Bullet;
typedef struct Ship Ship;

typedef struct Vertex {
    float x;
    float y;
};

// Structure definition for circlehitbox
typedef struct CircleHitbox {
    float xCenter;
    float yCenter;
    float rayon;
};

// Structure definition for polyhitbox
typedef struct PolyHitbox{
    Vertex vertices[MAX_VERTICES];
    int nbVertices;
};

// Structure definition for bullet
typedef struct Bullet {
    char filename[50];
    char id[50];
    int dmg;
    int xspeed;
    int yspeed;
    float x;
    float y;
    CircleHitbox hitbox;
};

// Structure definition for firingPattern (fp)
typedef struct FiringPattern {
    Bullet bullets[20];
};

// Structure definition for ship
typedef struct Ship {
    char filename[50];
    char id[50];
    int level;
    int nbLevels;
    FiringPattern fp[10];
    PolyHitbox hitbox;
};

void displayData();
void parseHitboxFile(const char* filename, Ship* ships, int* shipCount, Bullet* bullets, int* bulletCount);
int addBullets(SDL_Rect* bullets, SDL_Rect spaceshipRect, int numBullets, int shipLevel);

#endif