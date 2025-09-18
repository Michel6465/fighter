#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/* 
            DEFINITIONS
*/
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

// Cap FPS
#define FPS 120
#define FRAME_DELAY 1000/FPS // Durée d'une frame en millisecondes (16ms pour 60 FPS)

#define FIGHTER_WIDTH 40
#define FIGHTER_HEIGHT 80
#define FIGHTER_SPEED 2
#define FIGHTER_MAX_SPEED 20
#define FIGHTER_MASS 10.0f
#define MAX_BULLETS 1000

#define MENU_MARGIN_RIGHT 20
#define MENU_OFFSET 300

/* 
            FIGHTER STRUCTURES
*/
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


/* 
            MAP STRUCTURES
*/
typedef struct {
    SDL_Point position;    // World position
    int texture_index;     // Which star texture to use (0-9)
    float scale;           // Scale of the star (0.5 - 2.0)
    float rotation;        // Random rotation
    float brightness;      // Alpha/opacity (0.5 - 1.0)
} Star;

#define MAX_STARS 20000
#define STARFIELD_RADIUS 5000  // 10,000 px radius

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
#define GRAVITY_FACTOR 1e12

typedef struct {
    SDL_Point world_position;  // Position in world coordinates
    int texture_index;         // Which astral object texture to use (0-3)
    float scale;               // Scale factor (0.5 - 1.5)
    float rotation;            // Random rotation
    float angle;
    float distance;
    int w, h;
    int discovered;
    int score_value;
} AstralObject;

#define ASTRAL_TYPES 4        // 4 different types of astral objects

// Different quantities for each type
#define CLOUD_COUNT 5 
#define NEBULA_COUNT 3
#define NOVA_COUNT 4  
#define VORTEX_COUNT 2

#define TOTAL_ASTRAL_OBJECTS (CLOUD_COUNT + NEBULA_COUNT + NOVA_COUNT + VORTEX_COUNT)

// Score values for each type
#define CLOUD_SCORE 100  
#define NEBULA_SCORE 250
#define NOVA_SCORE 150   
#define VORTEX_SCORE 500 
typedef struct {
    int discovered_count[4];    // Count of discovered objects per type
    int total_count[4];         // Total objects per type
    int total_score[4];         // Total score per type
    int total_discovered;       // Total discovered objects
    int total_score_earned;     // Total score from discoveries
} DiscoverySystem;

typedef struct {
    Star stars[MAX_STARS];
    int num_stars;
    Planet planets[NUM_PLANETS];
    AstralObject astral_objects[TOTAL_ASTRAL_OBJECTS];
} BackgroundEffects;


/* 
            GAME STRUCTURES
*/
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
    int objectivesFinished;
    SDL_Rect bullets[MAX_BULLETS];
    const Uint8* keyState;
    DiscoverySystem discovery;
} Game;

typedef struct {
    SDL_Window* window;
    SDL_Texture* fighterTexture;
    SDL_Texture* thrusterTextures[4];
    SDL_Texture* pauseTexture;
    SDL_Texture* checkboxCheckedTexture;
    SDL_Texture* checkboxUncheckedTexture;
    SDL_Texture* pauseTexture2;
    SDL_Texture* checkboxCheckedTexture2;
    SDL_Texture* checkboxUncheckedTexture2;
    SDL_Texture* checkmarkTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* starTextures[10];
    SDL_Texture* planetTextures[NUM_PLANETS];
    SDL_Texture* astralTextures[4];
    SDL_Texture* menuBgTexture;
    SDL_Texture* menuBackground;
    SDL_Texture* optionsBackground;
    int num_star_textures;
    Mix_Music* music;
    Mix_Chunk* discoverySound;
    Mix_Chunk* wowSound;
    Mix_Chunk* aceSound;
    float musicVolume;      // Music volume (0.0 to 1.0)
    float soundEffectsVolume;
    TTF_Font* font;
    TTF_Font* titleFont;
    TTF_Font* uiFont;
    float bg_x, bg_y;
    int windowWidth, windowHeight;
    int isHoveringPause;
} GameResources;

enum {TYPE_BUTTON, TYPE_SLIDER, TYPE_CHECKBOX};

typedef struct {
    SDL_Color fillColor;
    int textJustify;
    int textAlign;
} Button;

typedef struct {
    SDL_Color innerColor;
    SDL_Color borderColor;
    SDL_Color knobColor;
    int innerHeight;
    float knobPosition;
    int length;
} Slider;

typedef struct {
    int boxSize;
    int isChecked;
    int textJustify;
    int textAlign;
} Checkbox;

typedef struct {
    Button button;
    Slider slider;
    Checkbox checkbox;
    int type;
    int w, h;
    char* text;
    SDL_Color textColor;
    SDL_Color hoverColor;
    int isHovering;
} MenuListItem;

typedef struct {
    MenuListItem* menuButtons;
    int nbMenuButtons;
    MenuListItem* optionsButtons;
    int nbOptionsButtons;
    SDL_Rect pauseButtonRect;
    SDL_Rect musicSliderRect;
    SDL_Rect musicSliderKnob;
    SDL_Rect sfxSliderRect; 
    SDL_Rect sfxSliderKnob;
    SDL_Rect musicTextRect;
    SDL_Rect sfxTextRect;
    SDL_Rect scoreRect;
    SDL_Rect titleRect;
    SDL_Color yellow;
    SDL_Color white;
    SDL_Color darkBlue;
    SDL_Color blue;
    int draggingMusic;
    int draggingSfx;
} UIElements;


/* 
            DECLARATIONS
*/
void checkInit(int val, const char* msg);
void checkInitMix(int condition, const char* message);

void initSDLSystems();
void initWindow(const char* title, GameResources* resources);
SDL_Renderer* initRenderer(SDL_Window* window);
TTF_Font* initFont(const char* fontPath, int size);
void initGameResources(SDL_Renderer* renderer, GameResources* resources);
void initUIElements(UIElements* ui, SDL_Window* window);
void initGame(Game* game);
void initFighter(Fighter* fighter, int windowWidth, int windowHeight);
void initSolarSystem(BackgroundEffects* bg_effects);
void generateStarfield(BackgroundEffects* bg_effects);
void initAstralObjects(BackgroundEffects* bg_effects, GameResources* resources);
void setupAstralObject(AstralObject* obj, int type, int spawn_radius, int score_value);
void initDiscoverySystem(Game* game);
void cleanupResources(GameResources* resources);

#endif