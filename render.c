#include "render.h"
#include "game.h"        // Needs Game and Fighter
#include "init.h"   // Needs resources and UI elements
#include "menu.h"        // Needs menu rendering functions
#include "sounds.h"
#include <stdio.h>

void renderMainMenu(SDL_Renderer* renderer, GameResources* resources, UIElements* ui) {
    // Render title
    renderText(renderer, resources->titleFont, "Fight game", ui->yellow, &ui->titleRect, 1, 1);

    // Render buttons
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a);
    SDL_RenderFillRect(renderer, &ui->startButtonRect);
    SDL_RenderFillRect(renderer, &ui->optionsButtonRect);
    SDL_RenderFillRect(renderer, &ui->quitButtonRect);

    // Render game_button borders
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a); // Black border
    SDL_RenderDrawRect(renderer, &ui->startButtonRect);
    SDL_RenderDrawRect(renderer, &ui->optionsButtonRect);
    SDL_RenderDrawRect(renderer, &ui->quitButtonRect);

    // Render button texts
    SDL_Rect startTextRect = { ui->startButtonRect.x + 10, ui->startButtonRect.y + 10, ui->startButtonRect.w - 20, ui->startButtonRect.h - 20 };
    SDL_Rect optionsTextRect = { ui->optionsButtonRect.x + 10, ui->optionsButtonRect.y + 10, ui->optionsButtonRect.w - 20, ui->optionsButtonRect.h - 20 };
    SDL_Rect quitTextRect = { ui->quitButtonRect.x + 10, ui->quitButtonRect.y + 10, ui->quitButtonRect.w - 20, ui->quitButtonRect.h - 20 };
    renderText(renderer, resources->font, "Start", ui->yellow, &startTextRect, 1, 1);
    renderText(renderer, resources->font, "Options", ui->yellow, &optionsTextRect, 1, 1);
    renderText(renderer, resources->font, "Quit", ui->yellow, &quitTextRect, 1, 1);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void renderOptionsScreen(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui) {
    // Render title
    renderText(renderer, resources->titleFont, "Options", ui->yellow, &ui->titleRect, 1, 1);

    if (game->isSound) SDL_RenderCopy(renderer, resources->checkboxCheckedTexture, NULL, &ui->checkbox1Rect);
    else SDL_RenderCopy(renderer, resources->checkboxUncheckedTexture, NULL, &ui->checkbox1Rect);

    if (game->isHard) SDL_RenderCopy(renderer, resources->checkboxCheckedTexture, NULL, &ui->checkbox2Rect);
    else SDL_RenderCopy(renderer, resources->checkboxUncheckedTexture, NULL, &ui->checkbox2Rect);

    // Render back button
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a); // Same color as other buttons
    SDL_RenderFillRect(renderer, &ui->backButtonRect);
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a); // Black border
    SDL_RenderDrawRect(renderer, &ui->backButtonRect);

    // Render texts
    SDL_Rect checkbox1TextRect = { ui->checkbox1Rect.x + ui->checkboxSize + 10, ui->checkbox1Rect.y, BUTTON_WIDTH, ui->checkboxSize };
    SDL_Rect checkbox2TextRect = { ui->checkbox2Rect.x + ui->checkboxSize + 10, ui->checkbox2Rect.y, BUTTON_WIDTH, ui->checkboxSize };
    SDL_Rect backTextRect = { ui->backButtonRect.x + 10, ui->backButtonRect.y + 10, ui->backButtonRect.w - 20, ui->backButtonRect.h - 20 };
    renderText(renderer, resources->font, "Sound", ui->yellow, &checkbox1TextRect, 1, 1);
    renderText(renderer, resources->font, "Hardcore", ui->yellow, &checkbox2TextRect, 1, 1);
    renderText(renderer, resources->font, "Back", ui->yellow, &backTextRect, 1, 1);

    // Render volume sliders
    renderVolumeSliders(renderer, resources, ui);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void renderGameplay(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects) {
    // Sprites
    // Render starfield first (far background)
    renderStarfield(renderer, bg_effects, resources);
    renderOrbitalTrails(renderer, bg_effects, resources);
    renderAstralObjects(renderer, bg_effects, resources);
    renderSolarSystem(renderer, bg_effects, resources);
    
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
    renderDiscoveryProgress(renderer, game, resources, ui);
    renderText(renderer, resources->font, scoreText, ui->yellow, &ui->scoreRect, 0, 0);
    
    // Pause and spaceship
    SDL_RenderCopy(renderer, resources->pauseTexture, NULL, &ui->pauseButtonRect);
}

void renderGameScreen(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui, BackgroundEffects* bg_effects) {
    // Clear screen
    SDL_RenderClear(renderer);

    switch (game->screen) {
        case MAIN_MENU:
            renderMainMenu(renderer, resources, ui);
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

void renderOrbitalTrails(SDL_Renderer* renderer, BackgroundEffects* bg_effects, GameResources* resources) {
    for (int i = 1; i < NUM_PLANETS; i++) {  // Skip sun
        Planet* planet = &bg_effects->planets[i];
        
        // Draw orbital path
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, 50);  // Semi-transparent blue
        float angle_increase = .1f / log(planet->orbit_radius);
        for (float angle = 0; angle < 2 * M_PI; angle += angle_increase) {
            int x1 = cos(angle) * planet->orbit_radius - resources->bg_x;
            int y1 = sin(angle) * planet->orbit_radius - resources->bg_y;
            int x2 = cos(angle + angle_increase) * planet->orbit_radius - resources->bg_x;
            int y2 = sin(angle + angle_increase) * planet->orbit_radius - resources->bg_y;
            
            if (abs(x1) < resources->windowWidth * 2 && abs(y1) < resources->windowHeight * 2) {
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void renderSolarSystem(SDL_Renderer* renderer, BackgroundEffects* bg_effects, GameResources* resources) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        Planet* planet = &bg_effects->planets[i];
        SDL_Texture* planetTexture = resources->planetTextures[planet->texture_index];
        
        if (!planetTexture) continue;
        
        // Get texture dimensions
        int original_w, original_h;
        SDL_QueryTexture(planetTexture, NULL, NULL, &original_w, &original_h);
        
        // Apply scaling
        int planet_width = planet->width /10;
        int planet_height = planet->width * original_h / original_w /10;
        
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
        int screen_x = world_x - resources->bg_x;
        int screen_y = world_y - resources->bg_y;
        
        // Only render if visible on screen
        if (screen_x + planet_width > -2000 && screen_x < resources->windowWidth + 2000 &&
            screen_y + planet_height > -2000 && screen_y < resources->windowHeight + 2000) {
            
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
                renderText(renderer, resources->font, planet->name, white, &name_rect, 1, 1);
            }
        }
    }
}

void renderStarfield(SDL_Renderer* renderer, BackgroundEffects* bg_effects, GameResources* resources) {
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
        int screen_x = star->position.x - resources->bg_x;
        int screen_y = star->position.y - resources->bg_y;
        
        // Only render if visible on screen (with some margin)
        if (screen_x + scaled_w > -100 && screen_x < resources->windowWidth + 100 &&
            screen_y + scaled_h > -100 && screen_y < resources->windowHeight + 100) {
            
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

void renderAstralObjects(SDL_Renderer* renderer, BackgroundEffects* bg_effects, GameResources* resources) {
    for (int i = 0; i < TOTAL_ASTRAL_OBJECTS; i++) {
        AstralObject* obj = &bg_effects->astral_objects[i];
        SDL_Texture* texture = resources->astralTextures[obj->texture_index];
        
        //if (!texture) continue;
        
        // Get texture dimensions if not already stored
        if (obj->w == 0 || obj->h == 0) {
            SDL_QueryTexture(texture, NULL, NULL, &obj->w, &obj->h);
        }
        
        // Apply scaling
        int scaled_w = obj->w * obj->scale /10;
        int scaled_h = obj->h * obj->scale /10;
        
        // Convert world to screen coordinates
        int screen_x = obj->world_position.x - resources->bg_x;
        int screen_y = obj->world_position.y - resources->bg_y;
        int center_x = screen_x + scaled_w / 2;
        int center_y = screen_y + scaled_h / 2;
        
        // Only render if visible on screen
        if (center_x + scaled_w/2 > -1000 && center_x - scaled_w/2 < resources->windowWidth + 1000 &&
            center_y + scaled_h/2 > -1000 && center_y - scaled_h/2 < resources->windowHeight + 1000) {

                SDL_Rect dest_rect = {screen_x, screen_y, scaled_w, scaled_h};
                SDL_RenderCopyEx(renderer, texture, NULL, &dest_rect, 
                                obj->rotation, NULL, SDL_FLIP_NONE);
            
            if (obj->discovered) {
                // Render discovered objects normally
                SDL_Rect dest_rect = {screen_x, screen_y, scaled_w, scaled_h};
                SDL_RenderCopyEx(renderer, texture, NULL, &dest_rect, 
                                obj->rotation, NULL, SDL_FLIP_NONE);
            } else {
                // Render circle around undiscovered objects
                int circle_radius = fminf(scaled_w, scaled_h) * 0.8f;
                
                // Pulsing effect for the circle
                Uint32 time = SDL_GetTicks();
                float pulse = (sin(time * 0.005f) + 1.0f) * 0.2f + 0.8f; // 0.8-1.2 pulse
                int pulsed_radius = circle_radius * pulse;
                
                // Color for the circle (bluish with transparency)
                SDL_Color circle_color = {100, 150, 255, 180}; // Semi-transparent blue
                
                // Draw the circle
                drawCircle(renderer, center_x, center_y, pulsed_radius, circle_color);
                
                // // Optional: Draw a second, smaller circle inside
                SDL_Color inner_circle_color = {150, 200, 255, 100};
                drawCircle(renderer, center_x, center_y, pulsed_radius * 0.7f, inner_circle_color);
                
                // Optional: Add a question mark or icon in the center
                SDL_Color text_color = {200, 200, 255, 200};
                SDL_Rect text_rect = {center_x - 10, center_y - 10, 20, 20};
                
                renderText(renderer, resources->uiFont, "?", text_color, &text_rect, 1, 1);
                
            }
        }
    }
}

void renderDiscoveryProgress(SDL_Renderer* renderer, Game* game, GameResources* resources, UIElements* ui) {
    char discovery_text[100];
    int shift;

    const int type_scores[4] = {CLOUD_SCORE, NEBULA_SCORE, NOVA_SCORE, VORTEX_SCORE};
    
    // Display total discovery progress
    sprintf(discovery_text, "Points objectifs :  %d", game->discovery.total_score_earned);
    renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {0, 60, 240, 30}, 0, 0);

    sprintf(discovery_text, "Decouvertes :");
    renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {0, 85, 240, 30}, 0, 0);

    sprintf(discovery_text, "%d", game->discovery.total_discovered);
    shift = floorf(game->discovery.total_discovered/10)==1 ? -13 : 0;
    renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {165 + shift, 85, 240, 30}, 0, 0);

    sprintf(discovery_text, "/%d", TOTAL_ASTRAL_OBJECTS);
    renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {185, 85, 240, 30}, 0, 0);
    
    // Display progress for each type
    const char* type_names[4] = {"Nuage", "Nebuleuse", "Supernova", "Vortex"};   
    for (int i = 0; i < 4; i++) {
        sprintf(discovery_text, "%s:", type_names[i]);
        renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {0, 150 + i * 30, 200, 30}, 0, 0);

        if (game->discovery.discovered_count[i] < game->discovery.total_count[i]) {
            sprintf(discovery_text, "%d", game->discovery.discovered_count[i]);
            renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {165, 150 + i * 30, 20, 30}, 0, 0);

            sprintf(discovery_text, "/%d", game->discovery.total_count[i]);
            renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {185, 150 + i * 30, 40, 30}, 0, 0);
        } else {
            SDL_RenderCopy(renderer, resources->checkmarkTexture, NULL, &(SDL_Rect) {175, 150 + i * 30, 30, 20});
        }

        sprintf(discovery_text, "(%d)", type_scores[i]);
        renderText(renderer, resources->uiFont, discovery_text, ui->white, &(SDL_Rect) {220, 150 + i * 30, 40, 30}, 0, 0);
    }
}

void renderVolumeSliders(SDL_Renderer* renderer, GameResources* resources, UIElements* ui) {
    // Music slider
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &ui->musicSliderRect);
    
    // Filled portion of music slider
    SDL_Rect musicFilled = {
        ui->musicSliderRect.x,
        ui->musicSliderRect.y,
        (int)(ui->musicSliderRect.w * resources->musicVolume),
        ui->musicSliderRect.h
    };
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a);
    SDL_RenderFillRect(renderer, &musicFilled);
    
    // SFX slider
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &ui->sfxSliderRect);
    
    // Filled portion of SFX slider
    SDL_Rect sfxFilled = {
        ui->sfxSliderRect.x,
        ui->sfxSliderRect.y,
        (int)(ui->sfxSliderRect.w * resources->soundEffectsVolume),
        ui->sfxSliderRect.h
    };
    SDL_SetRenderDrawColor(renderer, ui->darkBlue.r, ui->darkBlue.g, ui->darkBlue.b, ui->darkBlue.a);
    SDL_RenderFillRect(renderer, &sfxFilled);
    
    // Add outlines
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &ui->musicSliderRect);
    SDL_RenderDrawRect(renderer, &ui->sfxSliderRect);

    // Render knobs
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ui->musicSliderKnob);
    SDL_RenderFillRect(renderer, &ui->sfxSliderKnob);
    
    // Render slider labels
    renderText(renderer, resources->font, "Music:", ui->yellow, &ui->musicTextRect, 0, 1);
    renderText(renderer, resources->font, "SFX:", ui->yellow, &ui->sfxTextRect, 0, 1);
    
    // Render volume percentages
    char musicPercent[10], sfxPercent[10];
    sprintf(musicPercent, "%.0f%%", resources->musicVolume * 100);
    sprintf(sfxPercent, "%.0f%%", resources->soundEffectsVolume * 100);
    
    SDL_Rect musicPercentRect = {ui->musicSliderRect.x + ui->musicSliderRect.w + 10, ui->musicSliderRect.y, 40, 20};
    SDL_Rect sfxPercentRect = {ui->sfxSliderRect.x + ui->sfxSliderRect.w + 10, ui->sfxSliderRect.y, 40, 20};
    
    renderText(renderer, resources->font, musicPercent, ui->yellow, &musicPercentRect, 0, 1);
    renderText(renderer, resources->font, sfxPercent, ui->yellow, &sfxPercentRect, 0, 1);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void drawCircle(SDL_Renderer* renderer, int center_x, int center_y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // float angle_increase = .1f / log(radius);    
    for (int angle = 0; angle < 360; angle += 2) {
        float rad = angle * M_PI / 180.0f;
        int x1 = center_x + cos(rad) * radius;
        int y1 = center_y + sin(rad) * radius;
        int x2 = center_x + cos(rad + 0.035) * radius;
        int y2 = center_y + sin(rad + 0.035) * radius;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect, int centerHorizontally, int centerVertically) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) return;
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }
    
    // Create a new destination rect with proper aspect ratio
    SDL_Rect renderRect = {
        dstRect->x,
        dstRect->y,
        textSurface->w,  // Use actual text width
        textSurface->h   // Use actual text height
    };
    
    // Center horizontally if needed
    if (centerHorizontally && dstRect->w > renderRect.w) {
        renderRect.x = dstRect->x + (dstRect->w - renderRect.w) / 2;
    }
    
    // Center vertically if needed
    if (centerVertically && dstRect->h > renderRect.h) {
        renderRect.y = dstRect->y + (dstRect->h - renderRect.h) / 2;
    }

    int alignRight = 0;
    if (alignRight && dstRect->w > renderRect.w) {
        renderRect.x = dstRect->x + (dstRect->w - renderRect.w);
    }
    
    SDL_RenderCopy(renderer, textTexture, NULL, &renderRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}