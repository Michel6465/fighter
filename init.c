#include "init.h"
#include <stdio.h>

// Helper function for error checking
void checkInit(int condition, const char* message) {
    if (condition) {
        printf("Error: %s\n", message);
        exit(1);
    }
}

void checkInitMix(int condition, const char* message) {
    if (condition) {
        printf("Mix Error: %s\n", message);
        exit(1);
    }
}

void initSDLSystems() {
    // Initialize SDL
    checkInit(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0, "SDL could not initialize!");
    
    // Initialize SDL_ttf
    checkInit(TTF_Init() == -1, "SDL_ttf could not initialize!");
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    checkInit(!(IMG_Init(imgFlags) & imgFlags), "SDL_image could not initialize!");
    
    // Initialize SDL_mixer
    checkInitMix(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0, "SDL_mixer could not initialize!");
}

SDL_Window* initWindow(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(title,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        width,
                                        height,
                                        SDL_WINDOW_SHOWN);
    checkInit(!window, "Window could not be created!");
    return window;
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    checkInit(!renderer, "Renderer could not be created!");
    return renderer;
}

TTF_Font* initFont(const char* fontPath, int size) {
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    checkInit(!font, "Failed to load font!");
    return font;
}

void initGameResources(SDL_Renderer* renderer, GameResources* resources) {
    // Load checkbox images
    SDL_Surface* checkboxCheckedSurface = IMG_Load("img/menus/checkbox_checked.png");
    SDL_Surface* checkboxUncheckedSurface = IMG_Load("img/menus/checkbox_unchecked.png");
    checkInit(!checkboxCheckedSurface || !checkboxUncheckedSurface, "Failed to load checkbox images");

    resources->checkboxCheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxCheckedSurface);
    resources->checkboxUncheckedTexture = SDL_CreateTextureFromSurface(renderer, checkboxUncheckedSurface);
    SDL_FreeSurface(checkboxCheckedSurface);
    SDL_FreeSurface(checkboxUncheckedSurface);
    checkInit(!resources->checkboxCheckedTexture || !resources->checkboxUncheckedTexture, "Failed to create checkbox textures");

    // Load fighter image
    SDL_Surface* fighterSurface = IMG_Load("img/topdownfighter.png");
    checkInit(!fighterSurface, "Failed to load fighter image");
    resources->fighterTexture = SDL_CreateTextureFromSurface(renderer, fighterSurface);
    SDL_FreeSurface(fighterSurface);
    checkInit(!resources->fighterTexture, "Failed to create fighter texture");

    // Load pause button
    SDL_Surface* pauseSurface = IMG_Load("img/menus/pause2.png");
    checkInit(!pauseSurface, "Failed to load pause button image");
    resources->pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
    SDL_FreeSurface(pauseSurface);
    checkInit(!resources->pauseTexture, "Failed to create pause texture");

    // Load bullet image
    SDL_Surface* bulletSurface = IMG_Load("img/bullets/sprites_-_lasers_bullets_1_66v2.5/bullets_full/10.png");
    checkInit(!bulletSurface, "Failed to load bullet image");
    resources->bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    checkInit(!resources->bulletTexture, "Failed to create bullet texture");

    // Load thruster textures
    const int numberImages = 4;
    const char* thrusterPaths[] = {
        "img/thrusters/all/20 Thruster.png",
        "img/thrusters/all/19 Thruster.png", 
        "img/thrusters/all/18 Thruster.png",
        "img/thrusters/all/19 Thruster.png"
    };
    
    for (int i = 0; i < numberImages; i++) {
        SDL_Surface* thrusterSurface = IMG_Load(thrusterPaths[i]);
        if (!thrusterSurface) {
            printf("Warning: Failed to load thruster image %s\n", thrusterPaths[i]);
            // Create a placeholder texture
            thrusterSurface = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
            SDL_FillRect(thrusterSurface, NULL, SDL_MapRGB(thrusterSurface->format, 255, 100, 0));
        }
        resources->thrusterTextures[i] = SDL_CreateTextureFromSurface(renderer, thrusterSurface);
        SDL_FreeSurface(thrusterSurface);
        checkInit(!resources->thrusterTextures[i], "Failed to create thruster texture");
    }

    // Load star textures
    const char* starPaths[4] = {
        "img/stars/star1.png", "img/stars/star2.png", "img/stars/star4.png", "img/stars/star5.png"
    };
    
    resources->num_star_textures = 4;
    
    for (int i = 0; i < resources->num_star_textures; i++) {
        SDL_Surface* starSurface = IMG_Load(starPaths[i]);
        if (!starSurface) {
            printf("Warning: Failed to load star image %s\n", starPaths[i]);
            // Create a placeholder star
            starSurface = SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0);
            SDL_FillRect(starSurface, NULL, SDL_MapRGBA(starSurface->format, 255, 255, 255, 255));
            // Draw a simple star shape
            SDL_Rect center = {6, 6, 4, 4};
            SDL_FillRect(starSurface, &center, SDL_MapRGBA(starSurface->format, 200, 200, 100, 255));
        }
        resources->starTextures[i] = SDL_CreateTextureFromSurface(renderer, starSurface);
        SDL_SetTextureBlendMode(resources->starTextures[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(starSurface);
        checkInit(!resources->starTextures[i], "Failed to create star texture");
    }

    // Load planet textures
    const char* planetPaths[NUM_PLANETS] = {
        "img/solar_system/red_sun.png",      // 0 - Sun
        "img/solar_system/mercury.png",  // 1 - Mercury
        "img/solar_system/venus.png",    // 2 - Venus
        "img/solar_system/earth.png",    // 3 - Earth
        "img/solar_system/mars.png",     // 4 - Mars
        "img/solar_system/jupiter.png",  // 5 - Jupiter
        "img/solar_system/saturn.png",   // 6 - Saturn
        "img/solar_system/uranus.png",   // 7 - Uranus
        "img/solar_system/neptune.png"   // 8 - Neptune
    };
    
    for (int i = 0; i < NUM_PLANETS; i++) {
        SDL_Surface* planetSurface = IMG_Load(planetPaths[i]);
        if (!planetSurface) {
            printf("Warning: Failed to load planet image %s\n", planetPaths[i]);
            // Create placeholder
            planetSurface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
            SDL_Color colors[] = {{255, 200, 100}, {200, 150, 100}, {150, 150, 200}};
            SDL_FillRect(planetSurface, NULL, SDL_MapRGB(planetSurface->format, 
                colors[i % 3].r, colors[i % 3].g, colors[i % 3].b));
        }
        resources->planetTextures[i] = SDL_CreateTextureFromSurface(renderer, planetSurface);
        SDL_SetTextureBlendMode(resources->planetTextures[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(planetSurface);
        checkInit(!resources->planetTextures[i], "Failed to create planet texture");
    }

    // Load music
    resources->music = Mix_LoadMUS("music/pinball-theme.mp3");
    checkInitMix(resources->music == NULL, "Failed to load music");
    checkInitMix(Mix_PlayMusic(resources->music, -1) == -1, "Failed to play music");

    // Initialize fonts
    resources->font = initFont("sft.ttf", 28);
    resources->titleFont = initFont("sft.ttf", 48);

    // Initialize background position
    resources->bg_x = 0;
    resources->bg_y = 0;
}

void initUIElements(UIElements* ui) {
    // Title text dimensions
    ui->titleRect = (SDL_Rect){ (SCREEN_WIDTH - 400) / 2, 30, 400, 60 };
    
    // Button rectangles
    ui->startButtonRect = (SDL_Rect){ START_BUTTON_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->optionsButtonRect = (SDL_Rect){ OPTIONS_BUTTON_X, OPTIONS_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->quitButtonRect = (SDL_Rect){ QUIT_BUTTON_X, QUIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    
    // Options screen elements
    ui->checkbox1Rect = (SDL_Rect){ CHECKBOX_X, CHECKBOX_Y1, CHECKBOX_SIZE, CHECKBOX_SIZE };
    ui->checkbox2Rect = (SDL_Rect){ CHECKBOX_X, CHECKBOX_Y2, CHECKBOX_SIZE, CHECKBOX_SIZE };
    ui->backButtonRect = (SDL_Rect){ BACK_BUTTON_X, BACK_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    
    // Game screen elements
    ui->pauseButtonRect = (SDL_Rect){ SCREEN_WIDTH - 50 - 10, 10, 50, 50 };
    ui->scoreRect = (SDL_Rect){ 10, 10, 200, 50 };

    ui->titleColor = (SDL_Color){ 255, 255, 0, 0 };
    ui->buttonTextColor = (SDL_Color){ 255, 255, 0, 0 };
}

void initFighter(Fighter* fighter) {
    // Load spaceship image (texture should be loaded separately)
    fighter->speed_x = 0;
    fighter->speed_y = 0;
    fighter->x = SCREEN_WIDTH / 2 - FIGHTER_WIDTH / 2;
    fighter->y = SCREEN_HEIGHT / 2 - FIGHTER_HEIGHT / 2;
    fighter->angle = 0;
    fighter->rect = (SDL_Rect){ fighter->x, fighter->y, FIGHTER_WIDTH, FIGHTER_HEIGHT };

    // Initialize thruster state for dual thrusters
    fighter->thruster.current_frame = 0;
    fighter->thruster.is_visible = 0;
    fighter->thruster.last_update = 0;
    fighter->thruster.animation_speed = 100; // ms per frame
    
    // Position offsets for dual thrusters
    fighter->thruster.spread_distance = 6; // Distance from center
    fighter->thruster.left_offset.x = -fighter->thruster.spread_distance; // Left side
    fighter->thruster.left_offset.y = FIGHTER_HEIGHT / 2 + 5; // Below ship
    fighter->thruster.right_offset.x = fighter->thruster.spread_distance; // Right side
    fighter->thruster.right_offset.y = FIGHTER_HEIGHT / 2 + 5; // Below ship
}

void initSolarSystem(BackgroundEffects* bg_effects) {
    // Realistic relative distances and speeds (scaled for gameplay)
    PlanetDefinition planet_defs[NUM_PLANETS] = {
        // Sun (stationary at center)
        {"Sun",      0.0f,      0.0f,  40.0f,   3762,  27.4},  // radius, angle, speed, width, gravity
        
        // Planets with increasing distance and decreasing speed
        {"Mercury",  3200.0f,   0.0f,  17.7f,   392,   3.7},
        {"Venus",    4800.0f,   1.2f,  12.98f,  564,   8.87},
        {"Earth",    6400.0f,   2.4f,  11.04f,  576,   9.81},
        {"Mars",     8000.0f,   3.1f,  8.93f,   447,   3.73},
        {"Jupiter",  11200.0f,  4.5f,  8.9f,    1500,  24.79}, // rings
        {"Saturn",   14400.0f,  5.8f,  3.6f,    2788,  10.44},
        {"Uranus",   17600.0f,  0.7f,  2.53f,   2000,  8.69}, // rings
        {"Neptune",  20800.0f,  1.9f,  2.0f,    988,   11.15}
    };
    printf("%f\n", planet_defs[0].gravity);
    
    for (int i = 0; i < NUM_PLANETS; i++) {
        bg_effects->planets[i].orbit_radius = planet_defs[i].orbit_radius;
        bg_effects->planets[i].orbit_angle = planet_defs[i].start_angle;
        bg_effects->planets[i].orbit_speed = planet_defs[i].orbit_speed;
        bg_effects->planets[i].width = planet_defs[i].width;
        bg_effects->planets[i].mass = planet_defs[i].gravity;
        bg_effects->planets[i].texture_index = i;
        strncpy(bg_effects->planets[i].name, planet_defs[i].name, 19);
        bg_effects->planets[i].name[19] = '\0';
    }
    
    printf("Solar system initialized with %d planets\n", NUM_PLANETS);
}

void generateStarfield(BackgroundEffects* bg_effects) {
    bg_effects->num_stars = MAX_STARS;
    
    for (int i = 0; i < bg_effects->num_stars; i++) {
        // Random position within 10k radius circle
        float angle = (rand() % 360) * M_PI / 180.0f;
        float distance = (rand() % STARFIELD_RADIUS);
        
        bg_effects->stars[i].position.x = cos(angle) * distance;
        bg_effects->stars[i].position.y = sin(angle) * distance;
        
        // Random properties
        bg_effects->stars[i].texture_index = rand() % 10;
        bg_effects->stars[i].scale = 0.3f + (rand() % 70) / 100.0f;  // 0.3 - 1.0
        bg_effects->stars[i].rotation = rand() % 360;
        bg_effects->stars[i].brightness = 0.5f + (rand() % 50) / 100.0f;  // 0.5 - 1.0
    }
    
    printf("Generated %d stars in %d px radius\n", MAX_STARS, STARFIELD_RADIUS);
}

void cleanupResources(GameResources* resources) {
    if (resources->pauseTexture) SDL_DestroyTexture(resources->pauseTexture);
    if (resources->checkboxCheckedTexture) SDL_DestroyTexture(resources->checkboxCheckedTexture);
    if (resources->checkboxUncheckedTexture) SDL_DestroyTexture(resources->checkboxUncheckedTexture);
    if (resources->bulletTexture) SDL_DestroyTexture(resources->bulletTexture);
    if (resources->music) Mix_FreeMusic(resources->music);
    if (resources->font) TTF_CloseFont(resources->font);
    //if (resources->titleFont) TTF_CloseFont(resources->titleFont); // segfault maybe because the first closefont closes all the sizes at the same time
    
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}