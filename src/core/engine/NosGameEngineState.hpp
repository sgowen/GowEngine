//
//  NosGameEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

#include "core/input/InputState.hpp"
#include "core/network/MoveList.hpp"
#include "core/entity/EntityIDManager.hpp"
#include "core/entity/Entity.hpp"

enum NosGameInputProcessorState
{
    NGIMS_DEFAULT,
    NGIMS_DISPLAY_PHYSICS,
    NGIMS_EXIT,
    NGIMS_ZOOM_IN,
    NGIMS_ZOOM_OUT,
    NGIMS_ZOOM_RESET
};

class NosGameInputProcessor
{
public:
    NosGameInputProcessor();
    ~NosGameInputProcessor();
    
    NosGameInputProcessorState update();
    NosGameInputProcessorState state();
    void sampleInputAsNewMove(TimeTracker& tt);
    void removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed);
    InputState& inputState();
    MoveList& moveList();
    void reset();
    void setNumMovesProcessed(uint32_t numMovesProcessed);
    
private:
    MoveList _moveList;
    Pool<InputState> _poolInputState;
    InputState _inputState;
    NosGameInputProcessorState _state;
    uint32_t _numMovesProcessed;
    
    void drop2ndPlayer();
};

class World;

class NosGameEngineState : public EngineState
{    
public:    
    static NosGameEngineState& getInstance()
    {
        static NosGameEngineState ret = NosGameEngineState();
        return ret;
    }
    
    virtual void onEnter(Engine* e);
    virtual void onAssetsLoaded(Engine* e);
    virtual void onExit(Engine* e);
    virtual void onUpdate(Engine* e);
    virtual void onRender(Renderer& r, double extrapolation);
    
    void populateFromEntityLayout(EntityLayoutDef& eld);
    Entity* getPlayer(uint8_t playerID);
    Entity* getControlledPlayer();
    NosGameInputProcessor& input();
    std::map<uint8_t, Entity::PlayerInfo>& players();
    World& world();
    
private:
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    std::map<uint8_t, Entity::PlayerInfo> _players;
    World* _world;
    NosGameInputProcessor _inputProcessor;
    int _numMovesToReprocess;
    float _scale;
    
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
    
    void joinServer(Engine* e);
    
    void updateWithNetwork(Engine* e);
    void updateWorld(const Move& move);
    
    void renderWithNetwork(Renderer& r);
    
    void renderAudio();
    SoundFrameState& soundFrameStateAtMoveIndex(uint32_t moveIndex);
    void playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex);
    
    void spawnPlayerAndResetWorld();
    
    NosGameEngineState();
    virtual ~NosGameEngineState() {}
    NosGameEngineState(const NosGameEngineState&);
    NosGameEngineState& operator=(const NosGameEngineState&);
};
