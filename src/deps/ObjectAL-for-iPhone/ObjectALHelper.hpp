//
//  ObjectALHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#ifdef __OBJC__
#import "ObjectAL.h"
#endif

void initObjectAL();
void deinitObjectAL();
void pauseObjectAL();
void resumeObjectAL();
uint32_t loadSound(const char* bundleFilePath);
void unloadSound(uint32_t bufferKey);
void playSound(uint32_t bufferKey, bool isLooping);
void stopSound(uint32_t bufferKey);
void pauseSound(uint32_t bufferKey);
void resumeSound(uint32_t bufferKey);
void setSoundVolume(uint32_t bufferKey, float volume);
bool isSoundPlaying(uint32_t bufferKey);
bool isSoundLooping(uint32_t bufferKey);
void loadMusic(const char *bundleFilePath);
void unloadMusic();
void playMusic(bool isLooping);
void setMusicVolume(float volume);
void stopMusic();
void pauseMusic();
void resumeMusic();
bool isMusicPlaying();
bool isMusicLooping();
bool isMusicLoaded();
