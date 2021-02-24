//
//  ObjectALWrapper.mm
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ObjectALWrapper.hpp"

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
    
    // Create the device and context.
    // Note that it's easier to just let OALSimpleAudio handle
    // these rather than make and manage them yourself.
    gDevice = [ALDevice deviceWithDeviceSpecifier:nil];
    gContext = [ALContext contextOnDevice:gDevice attributes:nil];
    [OpenALManager sharedInstance].currentContext = gContext;
    
    // Deal with interruptions for me!
    [OALAudioSession sharedInstance].handleInterruptions = YES;
    
    // We don't want ipod music to keep playing since
    // we have our own bg music.
    [OALAudioSession sharedInstance].allowIpod = NO;
    
    // Mute all audio if the silent switch is turned on.
    [OALAudioSession sharedInstance].honorSilentSwitch = YES;
    
    // Take all 32 sources for this channel.
    // (we probably won't use that many but what the heck!)
    gChannel = [ALChannelSource channelWithSources:32];
    
    gMusicTrack = [OALAudioTrack track];
    
    gSounds = [[NSMutableDictionary alloc] init];
}

void deinitObjectAL()
{
    // Stop all music and sound effects.
    [gChannel stop];
    [gChannel clear];
    [gChannel resetToDefault];
    
    [gMusicTrack stop];
    [gSounds removeAllObjects];
    
    // Note: You'll likely only have one device and context open throughout
    // your program, so in a real program you'd be better off making a
    // singleton object that manages the device and context, rather than
    // allocating/deallocating it here.
    // Most of the demos just let OALSimpleAudio manage the device and context
    // for them.
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

uint32_t loadSound(const char* pathCString)
{
    NSString* path = [[NSString alloc] initWithCString:pathCString encoding:NSASCIIStringEncoding];
    
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

void loadMusic(const char* pathCString)
{
    NSString *path = [[NSString alloc] initWithCString:pathCString encoding:NSASCIIStringEncoding];
    
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
