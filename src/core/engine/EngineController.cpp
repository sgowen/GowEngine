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

void EngineController::configureForNetwork(std::string entityManagerFilePath, std::string entityLayoutManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config)
{
    static TimeTracker TIMC(getFrameRate());
    static EntityIDManager EIMC;
    static EntityLayout ELMC = EntityLayoutLoader::initWithJSONFile(entityLayoutManagerFilePath);
    
    INST_REG.registerInstance(INSK_TIME_CLNT, &TIMC);
    INST_REG.registerInstance(INSK_EID_CLNT, &EIMC);
    INST_REG.registerInstance(INSK_ELM_CLNT, &ELMC);
    
    configureForOffline(entityManagerFilePath, entityLayoutManagerFilePath, config);
}

void EngineController::configureForOffline(std::string entityManagerFilePath, std::string entityLayoutManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config)
{
    static TimeTracker TIMS(getFrameRate());
    static EntityIDManager EIMS;
    static EntityLayout ELMS = EntityLayoutLoader::initWithJSONFile(entityLayoutManagerFilePath);
    
    INST_REG.registerInstance(INSK_TIME_SRVR, &TIMS);
    INST_REG.registerInstance(INSK_EID_SRVR, &EIMS);
    INST_REG.registerInstance(INSK_ELM_SRVR, &ELMS);
    
    registerControllers(entityManagerFilePath, config);
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

void EngineController::registerControllers(std::string entityManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config)
{
    EntityManagerLoader::initWithJSONFile(ENTITY_MGR, entityManagerFilePath);
    
    for (auto& pair: config)
    {
        ENTITY_MGR.registerController(pair.first, pair.second);
    }
}
