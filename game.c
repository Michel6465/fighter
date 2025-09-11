#include "game.h"
#include "init.h"  // For GameResources
#include <math.h>
#include <stdio.h>

void handleKeyboardInput(Game* game, Fighter* fighter, GameResources* resources) {
    if (game->screen == GAME) {
        // Check for P key (pause) - use key press for one-time actions
        if (game->keyState[SDL_SCANCODE_P]) {
            printf("P key pressed - going back to main menu!\n");
            game->screen = MAIN_MENU;
            // Add a small delay to prevent multiple triggers
            SDL_Delay(200);
        }

        // Handle continuous movement keys
        if (game->keyState[SDL_SCANCODE_UP]) {
            float rad_angle = fighter->angle * PI / 180.0f;
            fighter->speed_x += sin(rad_angle) * FIGHTER_SPEED;
            fighter->speed_y += -cos(rad_angle) * FIGHTER_SPEED;
        }
        
        if (game->keyState[SDL_SCANCODE_DOWN]) {
            int action = getShortestRotationDirection(fighter);
            if (action == THRUST) { // accelerate if angle opposite to speed
                float rad_angle = fighter->angle * PI / 180.0f;
                fighter->speed_x += sin(rad_angle) * FIGHTER_SPEED;
                fighter->speed_y += -cos(rad_angle) * FIGHTER_SPEED;
            } else {
                switch(action) {
                    case TURN_LEFT:
                        fighter->angle -= ANGLES_PER_FRAME;
                        break;
                    case TURN_RIGHT:
                        fighter->angle += ANGLES_PER_FRAME;
                        break;
                    default: // DO_NOTHING
                        break;
                }
            }
        }
        
        if (game->keyState[SDL_SCANCODE_LEFT]) {
            fighter->angle -= ANGLES_PER_FRAME;
        }
        
        if (game->keyState[SDL_SCANCODE_RIGHT]) {
            fighter->angle += ANGLES_PER_FRAME;
        }
        
        if (game->keyState[SDL_SCANCODE_SPACE]) {
            fighter->speed_x = 0;
            fighter->speed_y = 0;
            fighter->angle = 0;
            resources->bg_x = 0;
            resources->bg_y = 0;
        }
    }
}

void updateGameState(Game* game, Fighter* fighter, GameResources* resources, UIElements* ui) {
    if (game->screen == GAME) {
        // Move bullets
        for (int i = 0; i < game->numBullets; i++) {
            game->bullets[i].x += 7; // Move bullet 1 pixel to the right

            // Remove bullets that move off-screen
            if (game->bullets[i].x > SCREEN_WIDTH) {
                for (int j = i; j < game->numBullets - 1; j++) {
                    game->bullets[j] = game->bullets[j + 1];
                }
                game->numBullets--;
                i--; // Recheck the current index
            }
        }

        // Update background position
        resources->bg_x -= fighter->speed_x;
        resources->bg_y -= fighter->speed_y;
        
        // Update fighter rectangle
        fighter->rect = (SDL_Rect){ fighter->x, fighter->y, FIGHTER_WIDTH, FIGHTER_HEIGHT };

        // Update score
        game->score++;
    }
}

// Helper function to find the shortest rotation direction movement direction and facing direction of the fighter
int getShortestRotationDirection(Fighter* fighter) {
    int currentAngle = fighter->angle;
    int targetAngle = ((int) getFighterMovementDirection(fighter) + 180) % 360; // [0, 360]
    
    currentAngle = currentAngle % 360; // [-360, 360]
    if (currentAngle < 0) currentAngle += 360; // [0, 360]
    //printf("%d, %d\n", targetAngle, currentAngle); 

    printf("angle:%d %d %d %d\n", currentAngle, targetAngle, abs(currentAngle - targetAngle), abs(currentAngle - targetAngle) < ANGLES_PER_FRAME + 0.01f);
    if (abs(currentAngle - targetAngle) < ANGLES_PER_FRAME + 0.01f) {
        fighter->angle = targetAngle;
        printf("movement:%f %d\n", getFighterMovementSpeed(fighter), getFighterMovementSpeed(fighter) < 0.2);
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
        printf("base angle %f\n", fighter->angle);
        return fighter->angle - 180;
    }
    //printf("not base angle\n");
    
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

so +90
*/

// out: speed > 0
float getFighterMovementSpeed(Fighter* fighter) {
    float sqrSpeed = fighter->speed_x*fighter->speed_x + fighter->speed_y*fighter->speed_y;
    return sqrt(sqrSpeed);
}