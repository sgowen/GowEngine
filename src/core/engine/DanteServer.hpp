//
//  DanteServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class Engine;
class ClientProxy;

class DanteServer
{
public:
    static void create();
    static DanteServer* getInstance();
    static void destroy();
    
    void update();
    void handleNewClient(std::string playerName, uint8_t playerID);
    void handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex);
    void resetWorld();
    void populateFromEntityLayout(EntityLayoutDef& eld);
    void restart();
    World& world();
    
private:
    static DanteServer* s_instance;
    
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    World* _world;
    bool _isRestarting;
    bool _isConnected;
    
    void updateWorld();
    void handleDirtyStates(std::vector<Entity*>& entities);
    void addPlayer(std::string playerName, uint8_t playerID);
    void removePlayer(uint8_t playerID);
    
    DanteServer();
    virtual ~DanteServer();
    DanteServer(const DanteServer&);
    DanteServer& operator=(const DanteServer&);
};
