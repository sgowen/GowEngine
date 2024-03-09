//
//  AudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class World;
class Entity;

class AudioEngine
{
public:
    static void create();
    static AudioEngine& getInstance();
    static void destroy();
    
    void pause();
    void resume();
    void render();
    void playSoundsForWorld(World& w);
    uint32_t playSound(std::string soundID, uint32_t numFramesSeekedAhead = 0, float volume = 1.0f, bool isLooping = false);
    void playMusic(std::string soundID, float volume = 1.0f, bool isLooping = true);
    void stopSound(std::string soundID);
    void pauseSound(std::string soundID);
    void resumeSound(std::string soundID);
    void stopAllSounds();
    void pauseAllSounds();
    void resumeAllSounds();
    bool isPaused();

private:
    static AudioEngine* s_instance;
    
    class SoundFrameState {
    public:
        std::map<uint32_t, std::map<std::string, uint32_t> > _entitySoundStates;
        uint32_t _frame;
        
        SoundFrameState() :
        _frame(0)
        {
            // Empty
        }
    };
    SoundFrameState _soundFrameStates[360];
    
    struct SoundCommand {
        uint32_t _numFramesSeekedAhead;
        float _volume;
        bool _isLooping;
        
        SoundCommand(uint32_t numFramesSeekedAhead,
                     float volume,
                     bool isLooping) :
        _numFramesSeekedAhead(numFramesSeekedAhead),
        _volume(volume),
        _isLooping(isLooping)
        {
            // Empty
        }
    };
    
    std::map<uint32_t, SoundCommand> _soundsToPlay;
    std::vector<uint32_t> _soundsToStop;
    std::vector<uint32_t> _soundsToPause;
    std::vector<uint32_t> _soundsToResume;
    bool _isPaused;
    
    SoundFrameState& soundFrameStateAtMoveIndex(uint32_t moveIndex);
    void playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex);
    
    AudioEngine();
    ~AudioEngine();
    AudioEngine(const AudioEngine&);
    AudioEngine& operator=(const AudioEngine&);
};
