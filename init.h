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

#define FIGHTER_WIDTH 40
#define FIGHTER_HEIGHT 80
#define FIGHTER_SPEED 0.5
#define FIGHTER_MAX_SPEED 10
#define FIGHTER_MASS 10.0f
#define MAX_BULLETS 1000
#define PI 3.14159265f

typedef struct {
    int current_frame;         // Current animation frame (0-3)
    int is_visible;            // Whether thrusters should be visible
    Uint32 last_update;        // Time of last animation update
    float animation_speed;     // Animation speed (ms per frame)
    SDL_Point left_offset;     // Position offset for left thruster
    SDL_Point right_offset;    // Position offset for right thruster
    float spread_distance;     // Distance from center for each thruster
} ThrusterState;

typedef struct {
    SDL_Texture* texture;
    int x, y;
    float speed_x, speed_y;
    float angle;
    SDL_Rect rect;
    ThrusterState thruster;
} Fighter;

typedef struct {
    SDL_Point position;    // World position
    int texture_index;     // Which star texture to use (0-9)
    float scale;           // Scale of the star (0.5 - 2.0)
    float rotation;        // Random rotation
    float brightness;      // Alpha/opacity (0.5 - 1.0)
} Star;

#define MAX_STARS 100000
#define STARFIELD_RADIUS 20000  // 10,000 px radius

typedef struct {
    SDL_Point position;    // Current screen position
    float orbit_radius;    // Distance from sun
    float orbit_angle;     // Current angle in orbit (radians)
    float orbit_speed;     // Radians per frame
    float width;           // Scale factor
    int texture_index;     // Which planet texture to use
    char name[20];         // Planet name
    float mass;            // Mass (proportional to gravity)
    float radius;          // Physical radius (for collision)
    SDL_Point world_pos;
} Planet;

typedef struct {
    char name[20];
    float orbit_radius;
    float start_angle;
    float orbit_speed;
    int width;
    float gravity;
} PlanetDefinition;

#define NUM_PLANETS 9  // Sun + 8 planets
#define GRAVITY_FACTOR 1e13

typedef struct {
    Star stars[MAX_STARS];
    int num_stars;
    Planet planets[NUM_PLANETS];
} BackgroundEffects;

typedef struct {
    SDL_Texture* fighterTexture;
    SDL_Texture* thrusterTextures[4];
    SDL_Texture* pauseTexture;
    SDL_Texture* checkboxCheckedTexture;
    SDL_Texture* checkboxUncheckedTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* starTextures[10];
    SDL_Texture* planetTextures[NUM_PLANETS];
    int num_star_textures;
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
void initSolarSystem(BackgroundEffects* bg_effects);
void cleanupResources(GameResources* resources);
void generateStarfield(BackgroundEffects* bg_effects);

#endif