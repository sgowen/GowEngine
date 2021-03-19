//
//  ObjectALHelper.mm
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "core/common/PlatformMacros.hpp"

#if IS_APPLE

#include "ObjectALHelper.hpp"

#import "ARCSafe_MemMgmt.h"

ALDevice* gDevice = nil;
ALContext* gContext = nil;
ALChannelSource* gChannel = nil;
OALAudioTrack* gMusicTrack = nil;
NSMutableDictionary* gSounds = nil;
uint32_t _bufferKey = 1;

@interface ObjectALSoundBufferWrapper : NSObject
{
    ALBuffer* mBuffer;
    id<ALSoundSource> mSoundSource;
}

- (id)initWithSoundPath:(NSString *)path;

- (void)play:(bool)isLooping;
- (void)stop;
- (void)pause;
- (void)resume;
- (void)setVolume:(float)volume;
- (bool)isPlaying;

@end

@implementation ObjectALSoundBufferWrapper

- (id)initWithSoundPath:(NSString *)path
{
    self = [super init];
    
    if (self)
    {
        mBuffer = [[OpenALManager sharedInstance] bufferFromFile:path];
        mSoundSource = nil;
    }
    
    return self;
}

- (void)play:(bool)isLooping
{
    mSoundSource = [gChannel play:mBuffer loop:isLooping];
}

- (void)stop
{
    if (mSoundSource)
    {
        [mSoundSource stop];
    }
}

- (void)pause
{
    if (mSoundSource)
    {
        [mSoundSource setPaused:true];
    }
}

- (void)resume
{
    if (mSoundSource)
    {
        [mSoundSource setPaused:false];
    }
}

- (void)setVolume:(float)volume
{
    if (mSoundSource)
    {
        [mSoundSource setVolume:volume];
    }
}

- (bool)isPlaying
{
    return mSoundSource && [mSoundSource playing];
}

- (bool)isLooping
{
    return mSoundSource && [mSoundSource looping];
}

@end

void initObjectAL()
{
    gSounds = nil;
    gMusicTrack = nil;
    gChannel = nil;
    gContext = nil;
    gDevice = nil;
    
    gDevice = [ALDevice deviceWithDeviceSpecifier:nil];
    gContext = [ALContext contextOnDevice:gDevice attributes:nil];
    [OpenALManager sharedInstance].currentContext = gContext;
    
    [OALAudioSession sharedInstance].handleInterruptions = YES;
    [OALAudioSession sharedInstance].allowIpod = NO;
    [OALAudioSession sharedInstance].honorSilentSwitch = YES;
    
    gChannel = [ALChannelSource channelWithSources:32];
    
    gMusicTrack = [OALAudioTrack track];
    
    gSounds = [[NSMutableDictionary alloc] init];
}

void deinitObjectAL()
{
    [gChannel stop];
    [gChannel clear];
    [gChannel resetToDefault];
    
    [gMusicTrack stop];
    [gSounds removeAllObjects];
    
    as_release(gSounds);
    as_release(gMusicTrack);
    as_release(gChannel);
    as_release(gContext);
    as_release(gDevice);
}

void pauseObjectAL()
{
    gMusicTrack.paused = YES;
    gChannel.paused = YES;
}

void resumeObjectAL()
{
    gChannel.paused = NO;
    gMusicTrack.paused = NO;
}

uint32_t loadSound(const char* bundleFilePath)
{
    NSString* path = [[NSString alloc] initWithCString:bundleFilePath encoding:NSASCIIStringEncoding];
    
    [gSounds setObject:[[ObjectALSoundBufferWrapper alloc] initWithSoundPath:path] forKey:[NSNumber numberWithInt:_bufferKey]];
    
    return _bufferKey++;
}

void unloadSound(uint32_t bufferKey)
{
    [gSounds removeObjectForKey:[NSNumber numberWithInt:bufferKey]];
}

void playSound(uint32_t bufferKey, bool isLooping)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    [sound play:isLooping];
}

void stopSound(uint32_t bufferKey)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    [sound stop];
}

void pauseSound(uint32_t bufferKey)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    [sound pause];
}

void resumeSound(uint32_t bufferKey)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    [sound resume];
}

void setSoundVolume(uint32_t bufferKey, float volume)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    [sound setVolume:volume];
}

bool isSoundPlaying(uint32_t bufferKey)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    return [sound isPlaying];
}

bool isSoundLooping(uint32_t bufferKey)
{
    ObjectALSoundBufferWrapper* sound = [gSounds objectForKey:[NSNumber numberWithInt:bufferKey]];
    
    return [sound isLooping];
}

void loadMusic(const char* bundleFilePath)
{
    NSString *path = [[NSString alloc] initWithCString:bundleFilePath encoding:NSASCIIStringEncoding];
    
    [gMusicTrack preloadFile:path];
}

void unloadMusic()
{
    [gMusicTrack clear];
}

void playMusic(bool isLooping)
{
    [gMusicTrack setNumberOfLoops:isLooping ? -1 : 0];
    
    [gMusicTrack play];
}

void setMusicVolume(float volume)
{
    [gMusicTrack setVolume:volume];
}

void stopMusic()
{
    [gMusicTrack stop];
}

void pauseMusic()
{
    [gMusicTrack setPaused:true];
}

void resumeMusic()
{
    [gMusicTrack setPaused:false];
}

bool isMusicPlaying()
{
    return [gMusicTrack playing];
}

bool isMusicLooping()
{
    return [gMusicTrack numberOfLoops] == -1;
}

bool isMusicLoaded()
{
    return [gMusicTrack preloaded];
}

#endif /* IS_APPLE */
