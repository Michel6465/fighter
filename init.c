#include "init.h"
#include <stdio.h>
#include <math.h>

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

void initWindow(const char* title, GameResources* resources) {
    SDL_Window* window = SDL_CreateWindow(title,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        1200,
                                        900,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP); // Make resizable
    checkInit(!window, "Window could not be created!");
    
    resources->window = window;  // Store window in resources
    SDL_GetWindowSize(resources->window, &(resources->windowWidth), &(resources->windowHeight));
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    checkInit(!renderer, "Renderer could not be created!");
    
    // Set logical size for consistent rendering regardless of window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetLogicalSize(renderer, w, h);
    
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

    SDL_Surface* checkmarkSurface = IMG_Load("img/menus/checkmark2.png");
    checkInit(!checkmarkSurface, "Failed to load checkmark image");
    resources->checkmarkTexture = SDL_CreateTextureFromSurface(renderer, checkmarkSurface);
    SDL_FreeSurface(checkmarkSurface);
    checkInit(!resources->checkmarkTexture, "Failed to create checkmark texture");

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
            planetSurface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
            SDL_Color colors[] = {{255, 200, 100}, {200, 150, 100}, {150, 150, 200}};
            #pragma GCC diagnostic pop
            SDL_FillRect(planetSurface, NULL, SDL_MapRGB(planetSurface->format, 
                colors[i % 3].r, colors[i % 3].g, colors[i % 3].b));
        }
        resources->planetTextures[i] = SDL_CreateTextureFromSurface(renderer, planetSurface);
        SDL_SetTextureBlendMode(resources->planetTextures[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(planetSurface);
        checkInit(!resources->planetTextures[i], "Failed to create planet texture");
    }

    // Load astral object textures
    const char* astralPaths[ASTRAL_TYPES] = {
        "img/stars/astral-objects/cloud.png",
        "img/stars/astral-objects/nebula.png", 
        "img/stars/astral-objects/nova.png",
        "img/stars/astral-objects/vortex.png"
    };
    
    for (int i = 0; i < ASTRAL_TYPES; i++) {
        SDL_Surface* astralSurface = IMG_Load(astralPaths[i]);
        if (!astralSurface) {
            printf("Warning: Failed to load astral image %s\n", astralPaths[i]);
            // Create placeholder
            astralSurface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
            SDL_Color colors[] = {{100, 50, 150}, {150, 100, 200}, {200, 150, 100}, {100, 200, 150}};
            #pragma GCC diagnostic pop
            SDL_FillRect(astralSurface, NULL, SDL_MapRGB(astralSurface->format, 
                colors[i].r, colors[i].g, colors[i].b));
        }
        resources->astralTextures[i] = SDL_CreateTextureFromSurface(renderer, astralSurface);
        SDL_SetTextureBlendMode(resources->astralTextures[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(astralSurface);
        checkInit(!resources->astralTextures[i], "Failed to create astral texture");
    }

    // Initialize volume levels
    resources->musicVolume = 0.5f;        // 30% volume for music
    resources->soundEffectsVolume = 0.5f; // 80% volume for sound effects
    
    // Set initial music volume
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * resources->musicVolume));

    // Load music
    resources->music = Mix_LoadMUS("music/pinball-theme.mp3");
    checkInitMix(resources->music == NULL, "Failed to load music");
    //checkInitMix(Mix_PlayMusic(resources->music, -1) == -1, "Failed to play music");

    // Load sound effects
    resources->discoverySound = Mix_LoadWAV("music/effects/ting.wav");
    resources->wowSound = Mix_LoadWAV("music/effects/wow.wav");
    resources->aceSound = Mix_LoadWAV("music/effects/ace.wav");
    if (!resources->discoverySound) printf("Warning: Failed to load discovery sound: %s\n", Mix_GetError());
    if (!resources->wowSound) printf("Warning: Failed to load wow sound: %s\n", Mix_GetError());
    if (!resources->aceSound) printf("Warning: Failed to load ace sound: %s\n", Mix_GetError());
    
    // Initialize fonts
    resources->uiFont = initFont("sft.ttf", 20);
    resources->font = initFont("sft.ttf", 28);
    resources->titleFont = initFont("sft.ttf", 48);

    // Initialize background position
    resources->bg_x = 0;
    resources->bg_y = 0;
}

void initUIElements(UIElements* ui, SDL_Window* window) {
    int screenWidth, screenHeight;
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);

    int startButtonX = (screenWidth - BUTTON_WIDTH) / 2;
    int startButtonY = screenHeight / 2 - (BUTTON_HEIGHT + screenHeight/50) * 1.5;

    int optionsButtonX = (screenWidth - BUTTON_WIDTH) / 2;
    int optionsButtonY = screenHeight / 2 - BUTTON_HEIGHT / 2;

    int quitButtonX = (screenWidth - BUTTON_WIDTH) / 2;
    int quitButtonY = screenHeight / 2 + (BUTTON_HEIGHT + screenHeight/50) / 2;

    int backButtonX = (screenWidth - BUTTON_WIDTH) / 2;
    int backButtonY = 300;

    int checkboxX = 150;
    int checkboxY1 = 150;
    int checkboxY2 = 200;
    ui->checkboxSize = 50;

    // Title text dimensions
    ui->titleRect = (SDL_Rect){ (screenWidth - 400) / 2, 30, 400, 60 };

    // Button rectangles
    ui->startButtonRect = (SDL_Rect){ startButtonX, startButtonY, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->optionsButtonRect = (SDL_Rect){ optionsButtonX, optionsButtonY, BUTTON_WIDTH, BUTTON_HEIGHT };
    ui->quitButtonRect = (SDL_Rect){ quitButtonX, quitButtonY, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Options screen elements
    ui->checkbox1Rect = (SDL_Rect){ checkboxX, checkboxY1, ui->checkboxSize, ui->checkboxSize };
    ui->checkbox2Rect = (SDL_Rect){ checkboxX, checkboxY2, ui->checkboxSize, ui->checkboxSize };
    ui->backButtonRect = (SDL_Rect){ backButtonX, backButtonY, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Volume sliders
    ui->musicSliderRect = (SDL_Rect){350, 300, 200, 20};
    ui->sfxSliderRect = (SDL_Rect){350, 350, 200, 20};
    
    // Initial knob positions (based on default volumes)
    ui->musicSliderKnob = (SDL_Rect){350 + 100, 295, 10, 30}; // 30% position
    ui->sfxSliderKnob = (SDL_Rect){350 + 100, 345, 10, 30};  // 80% position
    
    ui->musicTextRect = (SDL_Rect){250, 295, 90, 30};
    ui->sfxTextRect = (SDL_Rect){250, 345, 90, 30};

    ui->draggingMusic = 0;
    ui->draggingSfx = 0;

    // Game screen elements
    ui->pauseButtonRect = (SDL_Rect){ screenWidth - 50 - 10, 10, 50, 50 };
    ui->scoreRect = (SDL_Rect){ 10, 10, 200, 50 };

    ui->yellow = (SDL_Color){ 255, 230, 0, 0 };
    ui->white = (SDL_Color){ 255, 255, 255, 0 };
    ui->darkBlue = (SDL_Color){ 26, 28, 58, 0 };
    ui->blue = (SDL_Color){ 39, 44, 92, 0 };
}

void initGame(Game* game) {
    game->screen = MAIN_MENU;
    game->isSound = 1;
    game->isHard = 0;
    game->score = 0;
    game->shipLevel = 1;
    game->numBullets = 0;
    game->objectivesFinished = 0;
    game->keyState = SDL_GetKeyboardState(NULL);
}

void initFighter(Fighter* fighter, int windowWidth, int windowHeight) {
    // Load spaceship image (texture should be loaded separately)
    fighter->speed_x = 0;
    fighter->speed_y = 0;
    fighter->x = windowWidth / 2 - FIGHTER_WIDTH / 2;
    fighter->y = windowHeight / 2 - FIGHTER_HEIGHT / 2;
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
        {"Soleil",   0.0f,     0.0f,  40.0f,   3762,  27.4},  // radius, angle, speed, width, gravity
        
        // Planets with increasing distance and decreasing speed
        {"Mercure",  320.0f,   0.0f,  17.7f,   392,   3.7},
        {"Venus",    480.0f,   1.2f,  12.98f,  564,   8.87},
        {"Terre",    640.0f,   2.4f,  11.04f,  576,   9.81},
        {"Mars",     800.0f,   3.1f,  8.93f,   447,   3.73},
        {"Jupiter",  1120.0f,  4.5f,  8.9f,    1500,  24.79}, // rings
        {"Saturne",  1440.0f,  5.8f,  3.6f,    2788,  10.44},
        {"Uranus",   1760.0f,  0.7f,  2.53f,   2000,  8.69}, // rings
        {"Neptune",  2080.0f,  1.9f,  2.0f,    988,   11.15}
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
        float random_0_to_1 = (float)rand() / (float)RAND_MAX;
        float distance = sqrtf(random_0_to_1) * STARFIELD_RADIUS;
        
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

void initAstralObjects(BackgroundEffects* bg_effects, GameResources* resources) {
    const int SPAWN_RADIUS = 2000;
    int object_index = 0;
    int texture_w, texture_h;
    
    // Spawn Nebulae (Type 0)
    SDL_QueryTexture(resources->astralTextures[0], NULL, NULL, &texture_w, &texture_h);
    for (int i = 0; i < CLOUD_COUNT; i++) {
        AstralObject* obj = &bg_effects->astral_objects[object_index++];
        setupAstralObject(obj, 0, SPAWN_RADIUS, CLOUD_SCORE);
        obj->w = texture_w;
        obj->h = texture_h;
    }
    
    // Spawn Galaxies (Type 1)
    SDL_QueryTexture(resources->astralTextures[1], NULL, NULL, &texture_w, &texture_h);
    for (int i = 0; i < NEBULA_COUNT; i++) {
        AstralObject* obj = &bg_effects->astral_objects[object_index++];
        setupAstralObject(obj, 1, SPAWN_RADIUS, NEBULA_SCORE);
        obj->w = texture_w;
        obj->h = texture_h;
    }
    
    // Spawn Nebulae II (Type 2)
    SDL_QueryTexture(resources->astralTextures[2], NULL, NULL, &texture_w, &texture_h);
    for (int i = 0; i < NOVA_COUNT; i++) {
        AstralObject* obj = &bg_effects->astral_objects[object_index++];
        setupAstralObject(obj, 2, SPAWN_RADIUS, NOVA_SCORE);
        obj->w = texture_w;
        obj->h = texture_h;
    }
    
    // Spawn Galaxies II (Type 3)
    SDL_QueryTexture(resources->astralTextures[3], NULL, NULL, &texture_w, &texture_h);
    for (int i = 0; i < VORTEX_COUNT; i++) {
        AstralObject* obj = &bg_effects->astral_objects[object_index++];
        setupAstralObject(obj, 3, SPAWN_RADIUS, VORTEX_SCORE);
        obj->w = texture_w;
        obj->h = texture_h;
    }
    
    printf("Spawned astral objects: %d nebulae, %d galaxies, %d nebulae II, %d galaxies II\n",
           CLOUD_COUNT, NEBULA_COUNT, NOVA_COUNT, VORTEX_COUNT);
}

// Helper function to setup individual astral objects
void setupAstralObject(AstralObject* obj, int type, int spawn_radius, int score_value) {
    // Random position
    float angle = (rand() % 360) * M_PI / 180.0f;
    float random_0_to_1 = (float)rand() / (float)RAND_MAX;
    float distance = sqrtf(random_0_to_1) * spawn_radius;
    
    obj->world_position.x = cos(angle) * distance;
    obj->world_position.y = sin(angle) * distance;
    
    // Set properties based on type
    obj->texture_index = type;
    obj->score_value = score_value;
    obj->discovered = 0;
    
    // Random properties with type-specific ranges
    switch (type) {
        case 0: // Clouds
            obj->scale = 0.8f + (rand() % 40) / 100.0f;  // 0.8 - 1.2
            break;
        case 1: // Nebulae
            obj->scale = 0.8f + (rand() % 50) / 100.0f;  // 1.0 - 1.5
            break;
        case 2: // Novae
            obj->scale = 1.4f + (rand() % 60) / 100.0f;  // 0.7 - 1.3
            break;
        case 3: // Vortex (rare, larger)
            obj->scale = 0.8f + (rand() % 40) / 100.0f;  // 1.2 - 2.0
            break;
    }
    
    obj->rotation = rand() % 360;
}

void initDiscoverySystem(Game* game) {
    // Initialize counts to zero
    for (int i = 0; i < 4; i++) {
        game->discovery.discovered_count[i] = 0;
        game->discovery.total_count[i] = 0;
        game->discovery.total_score[i] = 0;
    }
    game->discovery.total_discovered = 0;
    game->discovery.total_score_earned = 0;
    
    // Set total counts based on our constants
    game->discovery.total_count[0] = CLOUD_COUNT;
    game->discovery.total_count[1] = NEBULA_COUNT;
    game->discovery.total_count[2] = NOVA_COUNT;
    game->discovery.total_count[3] = VORTEX_COUNT;
}

void cleanupResources(GameResources* resources) {
    if (resources->pauseTexture) SDL_DestroyTexture(resources->pauseTexture);
    if (resources->checkboxCheckedTexture) SDL_DestroyTexture(resources->checkboxCheckedTexture);
    if (resources->checkboxUncheckedTexture) SDL_DestroyTexture(resources->checkboxUncheckedTexture);
    if (resources->bulletTexture) SDL_DestroyTexture(resources->bulletTexture);
    if (resources->music) Mix_FreeMusic(resources->music);
    if (resources->uiFont) TTF_CloseFont(resources->uiFont);
    //if (resources->titleFont) TTF_CloseFont(resources->titleFont); // segfault maybe because the first closefont closes all the sizes at the same time
    
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}