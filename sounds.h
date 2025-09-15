#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <SDL2/SDL.h>
#include "init.h"

void setMusicVolume(GameResources* resources, float volume);
void setSoundEffectsVolume(GameResources* resources, float volume);
void playSound(Mix_Chunk* sound, GameResources* resources);

#endif