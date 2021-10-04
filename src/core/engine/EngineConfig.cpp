//
//  EngineConfig.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EngineConfig::EngineConfig(std::string configFilePath, void* data1, void* data2) :
_config(nullptr)
{
#if IS_ANDROID
    AndroidAssetHandler::create(data1, data2);
#elif IS_APPLE
    AppleAssetHandler::create(configFilePath);
#endif
    
    _config = new Config(ConfigLoader::initWithJSONFile(configFilePath));
    
    SOCKET_UTIL.setLoggingEnabled(_config->getBool("networkLoggingEnabled", false));
    INPUT_MGR.setLoggingEnabled(_config->getBool("inputLoggingEnabled", false));
    AUDIO_ENGINE.setSoundsDisabled(_config->getBool("soundsDisabled", false));
    AUDIO_ENGINE.setMusicDisabled(_config->getBool("musicDisabled", false));
    
    std::string filePathAssets = _config->getString("filePathAssets", "");
    if (!filePathAssets.empty())
    {
        ASSETS.registerAssets("engine", AssetsLoader::initWithJSONFile(filePathAssets));
    }
    
    static TimeTracker TIMS(getFrameRate());
    static EntityIDManager EIMS;
    INST_REG.registerInstance(INSK_TIME_SRVR, &TIMS);
    INST_REG.registerInstance(INSK_EID_SRVR, &EIMS);
    
    static TimeTracker TIMC(getFrameRate());
    static EntityIDManager EIMC;
    INST_REG.registerInstance(INSK_TIME_CLNT, &TIMC);
    INST_REG.registerInstance(INSK_EID_CLNT, &EIMC);
    
//    std::string filePathEntityLayout = _config->getString("filePathEntityLayout", "");
//    if (!filePathEntityLayout.empty())
//    {
//        static EntityLayout ELMS = EntityLayoutLoader::initWithJSONFile(filePathEntityLayout);
//        INST_REG.registerInstance(INSK_ELM_SRVR, &ELMS);
//        
//        static EntityLayout ELMC = EntityLayoutLoader::initWithJSONFile(filePathEntityLayout);
//        INST_REG.registerInstance(INSK_ELM_CLNT, &ELMC);
//    }
//    
//    std::string filePathEntityManager = _config->getString("filePathEntityManager", "");
//    if (!filePathEntityManager.empty())
//    {
//        EntityManagerLoader::initWithJSONFile(ENTITY_MGR, filePathEntityManager);
//        
//        for (auto& pair: entityControllers)
//        {
//            ENTITY_MGR.registerController(pair.first, pair.second);
//        }
//    }
}

EngineConfig::~EngineConfig()
{
    delete _config;
    
#if IS_ANDROID
    AndroidAssetHandler::destroy();
#elif IS_APPLE
    AppleAssetHandler::destroy();
#endif
}

std::string EngineConfig::getWindowTitle()
{
    return _config->getString("windowTitle", "GowEngineMount");
}

double EngineConfig::getFrameRate()
{
    return 1.0 / _config->getUInt("framesPerSecond", 60);
}
