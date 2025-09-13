#include "render.h"
#include "game.h"        // Needs Game and Fighter
#include "init.h"   // Needs resources and UI elements
#include "menu.h"        // Needs menu rendering functions
#include <stdio.h>

void renderMainMenu(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui) {
    // Render title
    renderText(renderer, resources->titleFont, "Fight game", ui->titleColor, &ui->titleRect);

    // Render buttons
    SDL_SetRenderDrawColor(renderer, 0x66, 0x00, 0x66, 0xFF);
    SDL_RenderFillRect(renderer, &ui->startButtonRect);
    SDL_RenderFillRect(renderer, &ui->optionsButtonRect);
    SDL_RenderFillRect(renderer, &ui->quitButtonRect);

    // Render bgame_utton borders
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black border
    SDL_RenderDrawRect(renderer, &ui->startButtonRect);
    SDL_RenderDrawRect(renderer, &ui->optionsButtonRect);
    SDL_RenderDrawRect(renderer, &ui->quitButtonRect);

    // Render button texts
    SDL_Rect startTextRect = { ui->startButtonRect.x + 10, ui->startButtonRect.y + 10, ui->startButtonRect.w - 20, ui->startButtonRect.h - 20 };
    SDL_Rect optionsTextRect = { ui->optionsButtonRect.x + 10, ui->optionsButtonRect.y + 10, ui->optionsButtonRect.w - 20, ui->optionsButtonRect.h - 20 };
    SDL_Rect quitTextRect = { ui->quitButtonRect.x + 10, ui->quitButtonRect.y + 10, ui->quitButtonRect.w - 20, ui->quitButtonRect.h - 20 };
    renderText(renderer, resources->font, "Start", ui->buttonTextColor, &startTextRect);
    renderText(renderer, resources->font, "Options", ui->buttonTextColor, &optionsTextRect);
    renderText(renderer, resources->font, "Quit", ui->buttonTextColor, &quitTextRect);
}

void renderOptionsScreen(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui) {
    // Render title
    renderText(renderer, resources->titleFont, "Options", ui->titleColor, &ui->titleRect);

    if (game->isSound) SDL_RenderCopy(renderer, resources->checkboxCheckedTexture, NULL, &ui->checkbox1Rect);
    else SDL_RenderCopy(renderer, resources->checkboxUncheckedTexture, NULL, &ui->checkbox1Rect);

    if (game->isHard) SDL_RenderCopy(renderer, resources->checkboxCheckedTexture, NULL, &ui->checkbox2Rect);
    else SDL_RenderCopy(renderer, resources->checkboxUncheckedTexture, NULL, &ui->checkbox2Rect);

    // Render back button
    SDL_SetRenderDrawColor(renderer, 0x66, 0x00, 0x66, 0xFF); // Same color as other buttons
    SDL_RenderFillRect(renderer, &ui->backButtonRect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black border
    SDL_RenderDrawRect(renderer, &ui->backButtonRect);

    // Render texts
    SDL_Rect checkbox1TextRect = { ui->checkbox1Rect.x + CHECKBOX_SIZE + 10, ui->checkbox1Rect.y, 200, CHECKBOX_SIZE };
    SDL_Rect checkbox2TextRect = { ui->checkbox2Rect.x + CHECKBOX_SIZE + 10, ui->checkbox2Rect.y, 200, CHECKBOX_SIZE };
    SDL_Rect backTextRect = { ui->backButtonRect.x + 10, ui->backButtonRect.y + 10, ui->backButtonRect.w - 20, ui->backButtonRect.h - 20 };
    renderText(renderer, resources->font, "Sound", ui->buttonTextColor, &checkbox1TextRect);
    renderText(renderer, resources->font, "Hardcore", ui->buttonTextColor, &checkbox2TextRect);
    renderText(renderer, resources->font, "Back", ui->buttonTextColor, &backTextRect);
}

void renderGameplay(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects) {
    // Sprites
    // Render starfield first (far background)
    renderStarfield(renderer, bg_effects, resources, resources->bg_x, resources->bg_y);
    renderOrbitalTrails(renderer, bg_effects, resources->bg_x, resources->bg_y);
    renderSolarSystem(renderer, bg_effects, resources, resources->bg_x, resources->bg_y);
    
    // Render thruster
    renderThruster(renderer, fighter, resources);

    // Fighter rotation - FIXED center calculation
    SDL_Point center = {fighter->rect.w / 2, fighter->rect.h / 2};
    SDL_RenderCopyEx(renderer, resources->fighterTexture, NULL, &fighter->rect, fighter->angle, &center, SDL_FLIP_NONE);

    // Render bullets
    for (int i = 0; i < game->numBullets; i++) {
        SDL_RenderCopy(renderer, resources->bulletTexture, NULL, &game->bullets[i]);
    }

    // UI
    // Render score
    char scoreText[20];
    sprintf(scoreText, "Score: %d", game->score);
    renderText(renderer, resources->font, scoreText, ui->titleColor, &ui->scoreRect);
    
    // Pause and spaceship
    SDL_RenderCopy(renderer, resources->pauseTexture, NULL, &ui->pauseButtonRect);
}

void renderGameScreen(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects) {
    // Clear screen
    SDL_RenderClear(renderer);

    switch (game->screen) {
        case MAIN_MENU:
            renderMainMenu(renderer, game, resources, ui);
            break;
        case OPTIONS:
            renderOptionsScreen(renderer, game, resources, ui);
            break;
        case GAME:
            renderGameplay(renderer, game, fighter, resources, ui, bg_effects);
            break;
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

void renderThruster(SDL_Renderer* renderer, Fighter* fighter, GameResources* resources) {
    if (!fighter->thruster.is_visible) return;
    
    // Get current thruster texture from resources
    int frame = fighter->thruster.current_frame;
    SDL_Texture* thrusterTexture = resources->thrusterTextures[frame];
    if (!thrusterTexture) return;
    
    // Render left thruster
    renderSingleThruster(renderer, thrusterTexture, fighter, fighter->thruster.left_offset);
    
    // Render right thruster
    renderSingleThruster(renderer, thrusterTexture, fighter, fighter->thruster.right_offset);
}

void renderSingleThruster(SDL_Renderer* renderer, SDL_Texture* texture, Fighter* fighter, SDL_Point offset) {
    if (!texture) return;
    
    // Get original thruster texture dimensions
    int original_w, original_h;
    SDL_QueryTexture(texture, NULL, NULL, &original_w, &original_h);
    
    // Apply scaling factor (e.g., 2.0x for twice as big)
    float scale_factor = 0.8f;
    int scaled_w = original_w * scale_factor;
    int scaled_h = original_h * scale_factor;
    
    // Calculate ship center
    SDL_Point ship_center = {
        fighter->rect.x + fighter->rect.w / 2,
        fighter->rect.y + fighter->rect.h / 2
    };
    
    // Calculate thruster position based on offset and ship rotation
    float rad_angle = fighter->angle * M_PI / 180.0f;
    
    // Rotate the offset by the ship's angle
    int rotated_x = offset.x * cos(rad_angle) - offset.y * sin(rad_angle);
    int rotated_y = offset.x * sin(rad_angle) + offset.y * cos(rad_angle);
    
    // Apply rotated offset to ship center
    SDL_Point thruster_pos = {
        ship_center.x + rotated_x,
        ship_center.y + rotated_y
    };
    
    // Create destination rectangle with scaled dimensions
    SDL_Rect dest_rect = {
        thruster_pos.x - scaled_w / 2,
        thruster_pos.y - scaled_h / 2,
        scaled_w,
        scaled_h
    };
    
    // Thruster should point opposite to ship direction (180° difference)
    float thruster_angle = fighter->angle + 90.0f;
    
    SDL_RenderCopyEx(renderer, texture, NULL, &dest_rect, 
                    thruster_angle, NULL, SDL_FLIP_NONE);
}
/*
void renderOrbitalTrails(SDL_Renderer* renderer, BackgroundEffects* bg_effects, float camera_x, float camera_y) {
    //for (int i = 1; i < NUM_PLANETS; i++) {  // Skip sun
        int i=1;
        Planet* planet = &bg_effects->planets[i];
        
        // Draw orbital path
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, 50);  // Semi-transparent blue
        for (float angle = 0; angle < 2 * M_PI; angle += 0.5f) {
            int x1 = cos(angle) * planet->orbit_radius - camera_x;
            int y1 = sin(angle) * planet->orbit_radius - camera_y;
            int x2 = cos(angle + 0.1f) * planet->orbit_radius - camera_x;
            int y2 = sin(angle + 0.1f) * planet->orbit_radius - camera_y;
            
            if (abs(x1) < SCREEN_WIDTH * 2 && abs(y1) < SCREEN_HEIGHT * 2) {
                printf("(%d, %d), (%d, %d), %f, %f\n", x1, y1, x2, y2, camera_x, camera_y);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    //}
    printf("\n");
}
*/

void renderOrbitalTrails(SDL_Renderer* renderer, BackgroundEffects* bg_effects, float camera_x, float camera_y) {
    for (int i = 1; i < NUM_PLANETS; i++) {  // Skip sun
        Planet* planet = &bg_effects->planets[i];
        
        // Draw orbital path
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, 50);  // Semi-transparent blue
        float angle_increase = .1f / log(planet->orbit_radius);
        for (float angle = 0; angle < 2 * M_PI; angle += angle_increase) {
            int x1 = cos(angle) * planet->orbit_radius - camera_x;
            int y1 = sin(angle) * planet->orbit_radius - camera_y;
            int x2 = cos(angle + angle_increase) * planet->orbit_radius - camera_x;
            int y2 = sin(angle + angle_increase) * planet->orbit_radius - camera_y;
            
            if (abs(x1) < SCREEN_WIDTH * 2 && abs(y1) < SCREEN_HEIGHT * 2) {
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void renderSolarSystem(SDL_Renderer* renderer, BackgroundEffects* bg_effects, 
                      GameResources* resources, float camera_x, float camera_y) {
    
    for (int i = 0; i < NUM_PLANETS; i++) {
        Planet* planet = &bg_effects->planets[i];
        SDL_Texture* planetTexture = resources->planetTextures[planet->texture_index];
        
        if (!planetTexture) continue;
        
        // Get texture dimensions
        int original_w, original_h;
        SDL_QueryTexture(planetTexture, NULL, NULL, &original_w, &original_h);
        
        // Apply scaling
        int planet_width = planet->width;
        int planet_height = planet->width * original_h / original_w;
        
        // Calculate world position
        float world_x, world_y;
        
        if (i == 0) {
            // Sun is stationary at world center (0,0)
            world_x = 0;
            world_y = 0;
        } else {
            // Planets orbit around sun
            world_x = cos(planet->orbit_angle) * planet->orbit_radius;
            world_y = sin(planet->orbit_angle) * planet->orbit_radius;
        }
        
        // Convert to screen coordinates
        int screen_x = world_x - camera_x;
        int screen_y = world_y - camera_y;
        
        // Only render if visible on screen
        if (screen_x + planet_width > -2000 && screen_x < SCREEN_WIDTH + 2000 &&
            screen_y + planet_height > -2000 && screen_y < SCREEN_HEIGHT + 2000) {
            
            SDL_Rect dest_rect = {
                screen_x - planet_width / 2,
                screen_y - planet_height / 2,
                planet_width,
                planet_height
            };
            
            // Render planet
            SDL_RenderCopyEx(renderer, planetTexture, NULL, &dest_rect, 
                            0, NULL, SDL_FLIP_NONE);
            
            // Optional: Render planet names (debug)
            if (strlen(planet->name) > 0) {
                SDL_Color white = {255, 255, 255, 255};
                SDL_Rect name_rect = {screen_x - 50, screen_y - planet_height/2 - 20, 100, 20};
                renderText(renderer, resources->font, planet->name, white, &name_rect);
            }
        }
    }
}

void renderStarfield(SDL_Renderer* renderer, BackgroundEffects* bg_effects, 
                    GameResources* resources, float camera_x, float camera_y) {
    for (int i = 0; i < bg_effects->num_stars; i++) {
        Star* star = &bg_effects->stars[i];
        SDL_Texture* starTexture = resources->starTextures[star->texture_index];
        
        if (!starTexture) continue;
        
        // Get original texture dimensions
        int original_w, original_h;
        SDL_QueryTexture(starTexture, NULL, NULL, &original_w, &original_h);
        
        // Apply scaling
        float base_scale_factor = 0.1f;
        int scaled_w = original_w * star->scale * base_scale_factor;
        int scaled_h = original_h * star->scale * base_scale_factor;
        
        // Calculate screen position (relative to camera)
        int screen_x = star->position.x - camera_x;
        int screen_y = star->position.y - camera_y;
        
        // Only render if visible on screen (with some margin)
        if (screen_x + scaled_w > -100 && screen_x < SCREEN_WIDTH + 100 &&
            screen_y + scaled_h > -100 && screen_y < SCREEN_HEIGHT + 100) {
            
            // Create destination rectangle
            SDL_Rect dest_rect = {
                screen_x - scaled_w / 2,
                screen_y - scaled_h / 2,
                scaled_w,
                scaled_h
            };
            
            // Set brightness/alpha
            SDL_SetTextureAlphaMod(starTexture, star->brightness * 255);
            
            // Render with rotation
            SDL_Point center = {scaled_w / 2, scaled_h / 2};
            SDL_RenderCopyEx(renderer, starTexture, NULL, &dest_rect, 
                            star->rotation, &center, SDL_FLIP_NONE);
            
            // Reset alpha for other textures
            SDL_SetTextureAlphaMod(starTexture, 255);
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, dstRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}