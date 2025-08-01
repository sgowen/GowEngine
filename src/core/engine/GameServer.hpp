//
//  GameServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityIDManager.hpp"
#include "core/common/TimeTracker.hpp"

class Engine;
class ClientProxy;

class GameServer
{
public:
    static void create();
    static GameServer* getInstance();
    static void destroy();
    
    void update();
    void handleNewClient(std::string playerName, uint8_t playerID);
    void handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex);
    void resetWorld();
    void populateFromEntityLayout(EntityLayout& el);
    void restart();
    World& world();
    
private:
    static GameServer* s_instance;
    
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    World* _world;
    bool _isRestarting;
    bool _isConnected;
    
    void updateWorld();
    void handleDirtyStates(std::vector<Entity*>& entities);
    void spawnPlayer(std::string playerName, uint8_t playerID);
    void removePlayer(uint8_t playerID);
    
    GameServer();
    ~GameServer();
    GameServer(const GameServer&);
    GameServer& operator=(const GameServer&);
};
