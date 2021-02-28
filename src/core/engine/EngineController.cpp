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
#include "PlatformMacros.hpp"
#include "deps/rapidjson/EntityManagerLoader.hpp"
#include "deps/rapidjson/EntityLayoutLoader.hpp"

#if IS_ANDROID
    #include "core/assets/AndroidAssetHandler.hpp"
#elif IS_APPLE
    #include "core/assets/AppleAssetHandler.hpp"
#endif

double EngineController::getFrameRate()
{
    return (1.0 / 60.0);
}

void EngineController::registerControllers(std::map<std::string, EntityControllerCreationFunc>& config, std::string entityManagerFilePath)
{
    EntityManagerLoader::initWithJSONFile(ENTITY_MGR, entityManagerFilePath);
    
    for (auto& pair: config)
    {
        ENTITY_MGR.registerController(pair.first, pair.second);
    }
}

void EngineController::configureForNetwork(std::map<std::string, EntityNetworkControllerCreationFunc>& config, std::string entityLayoutManagerFilePath)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerNetworkController(pair.first, pair.second);
    }
    
    static TimeTracker TIMS(getFrameRate());
    static TimeTracker TIMC(getFrameRate());
    static EntityIDManager EIMS;
    static EntityIDManager EIMC;
    static EntityLayout ELMS = EntityLayoutLoader::initWithJSONFile(entityLayoutManagerFilePath);
    static EntityLayout ELMC = EntityLayoutLoader::initWithJSONFile(entityLayoutManagerFilePath);
    
    INST_REG.registerInstance(INSK_TIME_SRVR, &TIMS);
    INST_REG.registerInstance(INSK_TIME_CLNT, &TIMC);
    INST_REG.registerInstance(INSK_EID_SRVR, &EIMS);
    INST_REG.registerInstance(INSK_EID_CLNT, &EIMC);
    INST_REG.registerInstance(INSK_ELM_SRVR, &ELMS);
    INST_REG.registerInstance(INSK_ELM_CLNT, &ELMC);
}

void EngineController::registerPhysicsControllers(std::map<std::string, EntityPhysicsControllerCreationFunc>& config)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerPhysicsController(pair.first, pair.second);
    }
}

void EngineController::registerRenderControllers(std::map<std::string, EntityRenderControllerCreationFunc>& config)
{
    for (auto& pair: config)
    {
        ENTITY_MGR.registerRenderController(pair.first, pair.second);
    }
}
    
EngineController::EngineController(void* data1, void* data2)
{
#if IS_ANDROID
    AndroidAssetHandler::create(data1, data2);
#elif IS_APPLE
    AppleAssetHandler::create(data1);
#endif
}

EngineController::~EngineController()
{
#if IS_ANDROID
    AndroidAssetHandler::destroy();
#elif IS_APPLE
    AppleAssetHandler::destroy();
#endif
}
