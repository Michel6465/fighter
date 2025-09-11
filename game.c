#include "game.h"

void parseHitboxFile(const char* filename, Ship* ships, int* shipCount, Bullet* bullets, int* bulletCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    *shipCount = 0;
    *bulletCount = 0;

    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        char id[50], type[10];
        if (sscanf(line, "%s %s", id, type) != 2) {
            continue;
        }

        if (strcmp(type, "poly") == 0) {
            // Parse a poly hitbox
            Ship* ship = &ships[(*shipCount)++];
            strcpy(ship->id, id);
            ship->hitbox.nbVertices = 0;

            char* token = strtok(line, " ");
            token = strtok(NULL, " "); // Skip id
            token = strtok(NULL, " "); // Skip type

            while (token != NULL) {
                if (ship->hitbox.nbVertices >= MAX_VERTICES) break;
                ship->hitbox.vertices[ship->hitbox.nbVertices].x = atof(token);
                token = strtok(NULL, " ");
                if (token == NULL) break;
                ship->hitbox.vertices[ship->hitbox.nbVertices].y = atof(token);
                token = strtok(NULL, " ");
                ship->hitbox.nbVertices++;
            }
        } else if (strcmp(type, "circle") == 0) {
            // Parse a circle hitbox
            Bullet* bullet = &bullets[(*bulletCount)++];
            strcpy(bullet->id, id);

            float xCenter, yCenter, rayon;
            sscanf(line, "%*s %*s %f %f %f", &xCenter, &yCenter, &rayon);

            bullet->hitbox.xCenter = xCenter;
            bullet->hitbox.yCenter = yCenter;
            bullet->hitbox.rayon = rayon;
        }
    }

    fclose(file);
}


int addBullets(SDL_Rect* bullets, SDL_Rect spaceshipRect, int numBullets, int shipLevel) {
    if (numBullets < MAX_BULLETS - 5) {
        switch (shipLevel) {
            case 1:
                bullets[numBullets].x = spaceshipRect.x + spaceshipRect.w - BULLET_SIZE; // Adjust x position as needed
                bullets[numBullets].y = spaceshipRect.y + (spaceshipRect.h / 2) - (BULLET_SIZE / 2) - 15; // Center bullet vertically
                bullets[numBullets].w = BULLET_SIZE; // Set bullet width
                bullets[numBullets].h = BULLET_SIZE; // Set bullet height
                numBullets++;
                break;
            case 2:
                bullets[numBullets].x = spaceshipRect.x + spaceshipRect.w - BULLET_SIZE; // Adjust x position as needed
                bullets[numBullets].y = spaceshipRect.y + (spaceshipRect.h / 2) - (BULLET_SIZE / 2) - 15; // Center bullet vertically
                bullets[numBullets].w = BULLET_SIZE; // Set bullet width
                bullets[numBullets].h = BULLET_SIZE; // Set bullet height
                numBullets++;
                bullets[numBullets].x = spaceshipRect.x + spaceshipRect.w - BULLET_SIZE; // Adjust x position as needed
                bullets[numBullets].y = spaceshipRect.y + (spaceshipRect.h / 2) - (BULLET_SIZE / 2) + 15; // Center bullet vertically
                bullets[numBullets].w = BULLET_SIZE; // Set bullet width
                bullets[numBullets].h = BULLET_SIZE; // Set bullet height
                numBullets++;
                break;
        }
    }

    return numBullets;
}

void displayData() {
    Ship ships[MAX_SHIPS];
    Bullet bullets[MAX_BULLETS];
    int shipCount = 0, bulletCount = 0;

    parseHitboxFile("data/hitboxes.data", ships, &shipCount, bullets, &bulletCount);

    // Print parsed ships
    printf("Parsed Ships: %d\n", shipCount);
    for (int i = 0; i < shipCount; i++) {
        printf("Ship %s: ", ships[i].id);
        for (int j = 0; j < ships[i].hitbox.nbVertices; j++) {
            printf("(%f, %f) ", ships[i].hitbox.vertices[j].x, ships[i].hitbox.vertices[j].y);
        }
        printf("\n");
    }

    // Print parsed bullets
    printf("Parsed Bullets: %d\n", bulletCount);
    for (int i = 0; i < bulletCount; i++) {
        printf("Bullet %s: center (%f, %f), radius %f\n", bullets[i].id, bullets[i].hitbox.xCenter, bullets[i].hitbox.yCenter, bullets[i].hitbox.rayon);
    }
}