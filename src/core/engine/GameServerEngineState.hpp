//
//  GameServerEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

class ClientProxy;

class GameServerEngineState : public State<Engine>
{
public:
    static GameServerEngineState& getInstance()
    {
        static GameServerEngineState ret = GameServerEngineState();
        return ret;
    }
    
    virtual void enter(Engine* e);
    virtual void execute(Engine* e);
    virtual void exit(Engine* e);
    
    void handleNewClient(std::string username, uint8_t playerID);
    void handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex);
    void resetWorld();
    void populateFromEntityLayout(EntityLayoutDef& eld);
    void restart();
    World& world();
    
private:
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    World* _world;
    bool _isRestarting;
    bool _isConnected;
    
    void update(Engine* e);
    void updateWorld();
    void handleDirtyStates(std::vector<Entity*>& entities);
    void addPlayer(std::string username, uint8_t playerID);
    void removePlayer(uint8_t playerID);
    
    GameServerEngineState();
    virtual ~GameServerEngineState() {}
    GameServerEngineState(const GameServerEngineState&);
    GameServerEngineState& operator=(const GameServerEngineState&);
};
