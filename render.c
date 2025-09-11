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

void renderGameplay(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui) {
    // Sprites
    // Fighter rotation - FIXED center calculation
    SDL_Point center = {fighter->rect.w / 2, fighter->rect.h / 2};

    SDL_Rect luna_rect = {(int) SCREEN_WIDTH/2 - LUNA_SIZE/2 + resources->bg_x, (int) SCREEN_HEIGHT/2 - LUNA_SIZE/2 + resources->bg_y, 2048, 2048};
    SDL_RenderCopy(renderer, resources->lunaTexture, NULL, &luna_rect);
    
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

void renderGameScreen(SDL_Renderer* renderer, Game* game, Fighter* fighter, GameResources* resources, UIElements* ui) {
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
            renderGameplay(renderer, game, fighter, resources, ui);
            break;
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* dstRect) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, dstRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}