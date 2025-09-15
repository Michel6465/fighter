#include "sounds.h"
#include <stdio.h>

void setMusicVolume(GameResources* resources, float volume) {
    if (!resources) return;
    
    volume = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
    resources->musicVolume = volume;
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * volume));
}

void setSoundEffectsVolume(GameResources* resources, float volume) {
    if (!resources) return;
    
    volume = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
    resources->soundEffectsVolume = volume;
    
    // Update volume of all currently playing sound effects
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        if (Mix_Playing(i)) {
            Mix_Volume(i, (int)(volume));
        }
    }
}

void playSound(Mix_Chunk* sound, GameResources* resources) {
    if (!sound || !resources) return;

    int volume = (int) (MIX_MAX_VOLUME * resources->soundEffectsVolume);
    Mix_VolumeChunk(sound, volume);
    
    // Use -1 to let SDL_mixer find any available channel
    int channel = Mix_PlayChannel(-1, sound, 0);
    
    if (channel == -1) {
        // If all channels are busy, try to play on channel 0 (will interrupt)
        Mix_HaltChannel(0);
        channel = Mix_PlayChannel(0, sound, 0);
        
        if (channel == -1) {
            printf("Completely failed to play sound: %s\n", Mix_GetError());
        }
    }
}