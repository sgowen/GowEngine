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
    std::string filePathAssets = _config.getString("filePathAssets", "");
    std::string filePathEntityLayoutManager = _config.getString("filePathEntityLayoutManager", "");
    std::string filePathEntityManager = _config.getString("filePathEntityManager", "");
    
    assert(!filePathAssets.empty());
    assert(!filePathEntityLayoutManager.empty());
    assert(!filePathEntityManager.empty());
    
    ASSETS.registerAssets("engine", AssetsLoader::initWithJSONFile(filePathAssets));
    EntityLayoutManagerLoader::initWithJSONFile(ENTITY_LAYOUT_MGR, filePathEntityLayoutManager);
    EntityManagerLoader::initWithJSONFile(ENTITY_MGR, filePathEntityManager);
    
    SOCKET_UTIL.setLoggingEnabled(_config.getBool("networkLoggingEnabled", false));
    INPUT_MGR.setLoggingEnabled(_config.getBool("inputLoggingEnabled", false));
    AUDIO_ENGINE.setSoundsDisabled(_config.getBool("soundsDisabled", false));
    AUDIO_ENGINE.setMusicDisabled(_config.getBool("musicDisabled", false));
    
    // FIXME
    // This is a silly thing to do
    // Physics controller used should be based on
    // the type of World class being used (e.g. Box2DWorld)
    ENTITY_MGR.registerPhysicsController("Default", Box2DPhysicsController::create);
}

EngineState& EngineConfig::initialEngineState()
{
    return _initialEngineState;
}
