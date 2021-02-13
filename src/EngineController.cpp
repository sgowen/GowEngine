//
//  EngineController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EngineController.hpp"

#include "Network.hpp"
#include "TimeTracker.hpp"
#include "EntityIDManager.hpp"
#include "InstanceRegistry.hpp"

#if IS_ANDROID
    #include "android/JNIAndroidAssetHandler.hpp"
#endif

double EngineController::getFrameRate()
{
    // default is 60 FPS
    return (1.0 / 60.0);
}

void EngineController::registerControllers(std::map<std::string, EntityControllerCreationFunc>& config)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerController(pair.first, pair.second);
    }
}

void EngineController::configureForNetwork(std::map<std::string, EntityNetworkControllerCreationFunc>& config)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerNetworkController(pair.first, pair.second);
    }
    
    static TimeTracker TIMS(getFrameRate());
    static TimeTracker TIMC(getFrameRate());
    static EntityIDManager EIMS;
    static EntityIDManager EIMC;
    
    INST_REG.registerInstance(INSK_TIME_SRVR, &TIMS);
    INST_REG.registerInstance(INSK_TIME_CLNT, &TIMC);
    INST_REG.registerInstance(INSK_EID_SRVR, &EIMS);
    INST_REG.registerInstance(INSK_EID_CLNT, &EIMC);
}

void EngineController::registerPhysicsControllers(std::map<std::string, EntityPhysicsControllerCreationFunc>& config)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerPhysicsController(pair.first, pair.second);
    }
}
    
EngineController::EngineController(void* data1, void* data2)
{
#if IS_ANDROID
    JNIAndroidAssetHandler::create(data1, data2);
#endif
}

EngineController::~EngineController()
{
#if IS_ANDROID
    JNIAndroidAssetHandler::destroy();
#endif
}
