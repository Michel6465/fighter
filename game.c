#include "game.h"
#include "init.h"  // For GameResources
#include "sounds.h"
#include <math.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

void updateGameState(Game* game, Fighter* fighter, GameResources* resources, BackgroundEffects* bg_effects) {
    if (game->screen == GAME) {
        /*
        // Move bullets
        for (int i = 0; i < game->numBullets; i++) {
            game->bullets[i].x += 7; // Move bullet 1 pixel to the right

            // Remove bullets that move off-screen
            if (game->bullets[i].x > resources->windowWidth) {
                for (int j = i; j < game->numBullets - 1; j++) {
                    game->bullets[j] = game->bullets[j + 1];
                }
                game->numBullets--;
                i--; // Recheck the current index
            }
        }
        */

        // Apply speed limit after all movement calculations
        limitFighterSpeed(fighter, FIGHTER_MAX_SPEED);

        updateSolarSystem(bg_effects);
        
        // calculateGravityForces(fighter, bg_effects, resources);

        // Update background position
        resources->bg_x += fighter->speed_x;
        resources->bg_y += fighter->speed_y;

        // Check for astral object discovery
        if (!game->objectivesFinished) {
            int new_discoveries = checkAstralObjectDiscovery(fighter, bg_effects, resources, game);
            
            if (new_discoveries > 0) {
                // Add score or other rewards for discovery
                game->score += new_discoveries * 100;
                
                if (resources->discoverySound) {
                    playSound(resources->discoverySound, resources); // Medium volume
                }
            }
            
            // Update fighter rectangle
            fighter->rect = (SDL_Rect){ fighter->x, fighter->y, FIGHTER_WIDTH, FIGHTER_HEIGHT };
        }
    }
}

void updateSolarSystem(BackgroundEffects* bg_effects) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        Planet* planet = &bg_effects->planets[i];

        // Skip the sun (index 0) as it's stationary
        if (i > 0) {
            // Update orbit angle
            planet->orbit_angle += planet->orbit_speed * SPEED_MULTIPLICATOR;
            if (planet->orbit_angle > 2 * M_PI) {
                planet->orbit_angle -= 2 * M_PI;
            }
        }

        // Update world position (sun at 0,0, planets orbit around it)
        if (i == 0) {
            planet->world_pos.x = 0;
            planet->world_pos.y = 0;
        } else {
            planet->world_pos.x = cos(planet->orbit_angle) * planet->orbit_radius;
            planet->world_pos.y = sin(planet->orbit_angle) * planet->orbit_radius;
        }
    }
}

// Helper function to find the shortest rotation direction movement direction and facing direction of the fighter
int getShortestRotationDirection(Fighter* fighter) {
    int currentAngle = fighter->angle;
    int targetAngle = ((int) getFighterMovementDirection(fighter) + 180) % 360; // [0, 360]
    
    currentAngle = currentAngle % 360; // [-360, 360]
    if (currentAngle < 0) currentAngle += 360; // [0, 360]

    if (abs(currentAngle - targetAngle) < ANGLES_PER_FRAME + 0.01f) {
        fighter->angle = targetAngle;
        if (getFighterMovementSpeed(fighter) < 0.2) {
            fighter->speed_x = 0;
            fighter->speed_y = 0;
            return DO_NOTHING;
        } else {
            return THRUST;
        }
    } else if (currentAngle < targetAngle) {
        return abs(targetAngle - currentAngle) < 180 ? TURN_RIGHT : TURN_LEFT;
    } else { // reverse direction if fighter angle > target angle
        return abs(targetAngle - currentAngle) < 180 ? TURN_LEFT : TURN_RIGHT;
    }
}

// Calculate the direction (angle) the fighter is moving based on velocity
float getFighterMovementDirection(Fighter* fighter) {
    // If the fighter is not moving, return current angle
    if (fabs(fighter->speed_x) < 0.01f && fabs(fighter->speed_y) < 0.01f) {
        return fighter->angle - 180;
    }
    
    // Calculate angle from velocity vector using atan2
    // atan2(y, x) gives angle in radians, convert to degrees
    float direction = atan2f(fighter->speed_y, fighter->speed_x) * (180.0f / M_PI) +90;
    
    // Convert from [-180, 180] range to [0, 360] range
    if (direction < 0) {
        direction += 360;
    }
    // printf("target angle: %f\n", direction);
    return direction;
}

/*
atan2: 
         270°
    180°      0°
          90°

my system:
          0°
    270°      90°
         180°

so +90°
*/

// out: speed > 0
float getFighterMovementSpeed(Fighter* fighter) {
    float sqrSpeed = fighter->speed_x*fighter->speed_x + fighter->speed_y*fighter->speed_y;
    return sqrtf(sqrSpeed);
}

void updateThruster(ThrusterState* thruster, int is_thrusting) {
    Uint32 current_time = SDL_GetTicks();
    
    // Show/hide thrusters based on thrusting state
    thruster->is_visible = is_thrusting;
    
    if (is_thrusting) {
        // Animate thruster only when visible
        if (current_time - thruster->last_update > thruster->animation_speed) {
            thruster->current_frame = (thruster->current_frame + 1) % 3;
            thruster->last_update = current_time;
        }
    } else {
        // Reset to first frame when not thrusting
        thruster->current_frame = 0;
    }
}

void limitFighterSpeed(Fighter* fighter, float max_speed) {
    float current_speed = getFighterMovementSpeed(fighter);
    
    if (current_speed > max_speed) {
        // Normalize the velocity vector and scale to max speed
        float ratio = max_speed / current_speed;
        fighter->speed_x *= ratio;
        fighter->speed_y *= ratio;
    }
}

void calculateGravityForces(Fighter* fighter, BackgroundEffects* bg_effects, GameResources* resources) {
    const float G = 6.67e-11;
    
    // Reset acceleration
    float accel_x = 0;
    float accel_y = 0;
    
    // Fighter position in world coordinates
    float fighter_x = resources->bg_x + fighter->x + fighter->rect.w / 2;
    float fighter_y = resources->bg_y + fighter->y + fighter->rect.h / 2;
    
    for (int i = 0; i < NUM_PLANETS; i++) {
        Planet* planet = &bg_effects->planets[i];
        
        // Calculate distance between fighter and planet
        float dx = planet->world_pos.x - fighter_x;
        float dy = planet->world_pos.y - fighter_y;
        float distance_squared = dx * dx + dy * dy;
        float distance = sqrtf(distance_squared);
        float force_magnitude, acceleration;
        
        // sun: 4000/27.4 = 146.00
        if (distance < 146*planet->mass) {
            distance = sqrtf(distance_squared + 200*200); // softening

            // Calculate gravitational force (F = G * m1 * m2 / r²)
            force_magnitude = (G * planet->mass * FIGHTER_MASS) / distance_squared /5;
            
            // Convert force to acceleration (a = F / m)
            acceleration = force_magnitude / FIGHTER_MASS;
            
            // Normalize direction vector and apply acceleration
            accel_x += (dx / distance) * acceleration * GRAVITY_FACTOR;
            accel_y += (dy / distance) * acceleration * GRAVITY_FACTOR;

            if (distance < 0.05*planet->mass) break; // ignore other plantes gravity if close to the center
        }
    }

    // Apply acceleration to fighter velocity
    fighter->speed_x += accel_x;
    fighter->speed_y += accel_y;
}

int checkAstralObjectDiscovery(Fighter* fighter, BackgroundEffects* bg_effects, GameResources* resources, Game* game) {
    int new_discoveries = 0;
    
    // Fighter center in world coordinates
    float fighter_center_x = resources->bg_x + fighter->x + fighter->rect.w / 2;
    float fighter_center_y = resources->bg_y + fighter->y + fighter->rect.h / 2;
    
    for (int i = 0; i < TOTAL_ASTRAL_OBJECTS; i++) {
        AstralObject* obj = &bg_effects->astral_objects[i];

        // Skip already discovered objects
        if (obj->discovered) continue;

        int scaled_w = obj->w * obj->scale /10;
        int scaled_h = obj->h * obj->scale /10;
        
        // Calculate distance between centers
        float dx = obj->world_position.x + scaled_w/2 - fighter_center_x;
        float dy = obj->world_position.y + scaled_h/2 - fighter_center_y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        // Check if fighter is close enough (within 100px)
        if (distance < fminf(obj->w, obj->h) * obj->scale * 0.8f /10) {
            // Mark as discovered and update scores
            obj->discovered = 1;
            int type = obj->texture_index;
            
            game->discovery.discovered_count[type]++;
            game->discovery.total_score[type] += obj->score_value;
            game->discovery.total_discovered++;
            game->discovery.total_score_earned += obj->score_value;
            game->score += obj->score_value;
            
            new_discoveries++;
        }
    }

    if (game->discovery.total_discovered == TOTAL_ASTRAL_OBJECTS) {
        if (rand()%3 < 2) playSound(resources->aceSound, resources);
        else              playSound(resources->wowSound, resources);
        game->objectivesFinished = 1;
    }
    
    return new_discoveries;
}

