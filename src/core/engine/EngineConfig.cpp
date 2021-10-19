//
//  EngineConfig.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EngineConfig::EngineConfig(std::string configFilePath, EngineState& initialEngineState) :
_config(ConfigLoader::initWithJSONFile(configFilePath)),
_initialEngineState(initialEngineState)
{
    ASSETS_MGR.registerAssets("engine", AssetsLoader::initWithJSONFile(_config.getString("filePathAssets")));
    EntityLayoutManagerLoader::initWithJSONFile(ENTITY_LAYOUT_MGR, _config.getString("filePathEntityLayoutManager"));
    EntityManagerLoader::initWithJSONFile(ENTITY_MGR, _config.getString("filePathEntityManager"));
    
    LOGGING_UTIL.setGlfwLoggingEnabled(_config.getBool("glfwLoggingEnabled", false));
    LOGGING_UTIL.setInputLoggingEnabled(_config.getBool("inputLoggingEnabled", false));
    LOGGING_UTIL.setNetworkLoggingEnabled(_config.getBool("networkLoggingEnabled", false));
    LOGGING_UTIL.setPhysicsLoggingEnabled(_config.getBool("physicsLoggingEnabled", false));
    
    AUDIO_ENGINE.setSoundsDisabled(_config.getBool("soundsDisabled", false));
    AUDIO_ENGINE.setMusicDisabled(_config.getBool("musicDisabled", false));
    
    // FIXME
    // This is a silly thing to do
    // Physics controller used should be based on
    // the type of World class being used (e.g. Box2DPhysicsWorld)
    ENTITY_MGR.registerPhysicsController("Default", Box2DPhysicsController::create);
}

Config& EngineConfig::config()
{
    return _config;
}

EngineState& EngineConfig::initialEngineState()
{
    return _initialEngineState;
}
