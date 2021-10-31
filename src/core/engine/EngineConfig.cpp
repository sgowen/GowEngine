//
//  EngineConfig.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EngineConfig* EngineConfig::s_instance = nullptr;

void EngineConfig::create(std::string configFilePath)
{
    assert(s_instance == nullptr);
    
    s_instance = new EngineConfig(configFilePath);
}

EngineConfig& EngineConfig::getInstance()
{
    assert(s_instance != nullptr);
    
    return *s_instance;
}

void EngineConfig::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

uint8_t EngineConfig::glfwSwapInterval()
{
    return _config.getUInt("glfwSwapInterval", 1);
}

uint8_t EngineConfig::framesPerSecond()
{
    return _config.getUInt("framesPerSecond", 60);
}

std::string EngineConfig::filePathEngineAssets()
{
    return _config.getString("filePathEngineAssets");
}

std::string EngineConfig::filePathEntityLayoutManager()
{
    return _config.getString("filePathEntityLayoutManager");
}

std::string EngineConfig::filePathEntityManager()
{
    return _config.getString("filePathEntityManager");
}

bool EngineConfig::glfwLoggingEnabled()
{
    return _config.getBool("glfwLoggingEnabled", false);
}

bool EngineConfig::inputLoggingEnabled()
{
    return _config.getBool("inputLoggingEnabled", false);
}

bool EngineConfig::networkLoggingEnabled()
{
    return _config.getBool("networkLoggingEnabled", false);
}

bool EngineConfig::physicsLoggingEnabled()
{
    return _config.getBool("physicsLoggingEnabled", false);
}

bool EngineConfig::soundsDisabled()
{
    return _config.getBool("soundsDisabled", false);
}

bool EngineConfig::musicDisabled()
{
    return _config.getBool("musicDisabled", false);
}

uint8_t EngineConfig::maxNumPlayers()
{
    return _config.getUInt("maxNumPlayers", 2);
}

uint8_t EngineConfig::maxTextInputLength()
{
    return _config.getUInt("maxTextInputLength", 16);
}

uint8_t EngineConfig::maxNumSoundsToPlayPerFrame()
{
    return _config.getUInt("maxNumSoundsToPlayPerFrame", 3);
}

uint16_t EngineConfig::clientPortHost()
{
    return _config.getUInt("clientPortHost", 1337);
}

uint16_t EngineConfig::clientPortJoin()
{
    return _config.getUInt("clientPortJoin", 1338);
}

uint16_t EngineConfig::serverPort()
{
    return _config.getUInt("serverPort", 9999);
}

uint8_t EngineConfig::maxNumMoves()
{
    return _config.getUInt("maxNumMoves", 7);
}

uint8_t EngineConfig::maxNumPacketsToProcessPerFrame()
{
    return _config.getUInt("maxNumPacketsToProcessPerFrame", maxNumPlayers() * 2);
}

uint8_t EngineConfig::numFramesOfSimulatedLatency()
{
    return _config.getUInt("numFramesOfSimulatedLatency", 2);
}

float EngineConfig::frameRate()
{
    return _frameRate;
}

uint8_t EngineConfig::timeScale()
{
    return _timeScale;
}

EngineConfig::EngineConfig(std::string configFilePath) :
_config(ConfigLoader::initWithJSONFile(configFilePath)),
_frameRate(1.0f / framesPerSecond()),
_timeScale(framesPerSecond() == 60 ? 2 : framesPerSecond() == 30 ? 1 : 0)
{
    assert(_timeScale > 0);
}

EngineConfig::~EngineConfig()
{
    // Empty
}
