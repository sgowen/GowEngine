//
//  ObjectALWrapper.hpp
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
int loadSound(const char* pathCString);
void playSound(int bufferIndex, bool isLooping);
void stopSound(int bufferIndex);
void pauseSound(int bufferIndex);
void resumeSound(int bufferIndex);
void setSoundVolume(int bufferIndex, float volume);
bool isSoundPlaying(int bufferIndex);
bool isSoundLooping(int bufferIndex);
void loadMusic(const char *pathCString);
void playMusic(bool isLooping);
void setMusicVolume(float volume);
void stopMusic();
void pauseMusic();
void resumeMusic();
bool isMusicPlaying();
bool isMusicLooping();
bool isMusicLoaded();
void unloadMusic();
