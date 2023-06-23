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
    return _glfwSwapInterval;
}

uint8_t EngineConfig::framesPerSecond()
{
    return _framesPerSecond;
}

std::string EngineConfig::filePathEngineAssets()
{
    return _filePathEngineAssets;
}

std::string EngineConfig::filePathEntityLayoutManager()
{
    return _filePathEntityLayoutManager;
}

std::string EngineConfig::filePathEntityManager()
{
    return _filePathEntityManager;
}

bool EngineConfig::glfwLoggingEnabled()
{
    return _glfwLoggingEnabled;
}

bool EngineConfig::fileLoggingEnabled()
{
    return _fileLoggingEnabled;
}

bool EngineConfig::inputLoggingEnabled()
{
    return _inputLoggingEnabled;
}

bool EngineConfig::networkLoggingEnabled()
{
    return _networkLoggingEnabled;
}

bool EngineConfig::physicsLoggingEnabled()
{
    return _physicsLoggingEnabled;
}

bool EngineConfig::soundsDisabled()
{
    return _soundsDisabled;
}

bool EngineConfig::musicDisabled()
{
    return _musicDisabled;
}

uint8_t EngineConfig::maxTextInputLength()
{
    return _maxTextInputLength;
}

uint8_t EngineConfig::maxNumSoundsToPlayPerFrame()
{
    return _maxNumSoundsToPlayPerFrame;
}

uint16_t EngineConfig::clientPortHost()
{
    return _clientPortHost;
}

uint16_t EngineConfig::clientPortJoin()
{
    return _clientPortJoin;
}

uint16_t EngineConfig::serverPort()
{
    return _serverPort;
}

uint8_t EngineConfig::maxNumMoves()
{
    return _maxNumMoves;
}

uint8_t EngineConfig::maxNumPacketsToProcessPerFrame()
{
    return _maxNumPacketsToProcessPerFrame;
}

uint8_t EngineConfig::numFramesOfSimulatedLatency()
{
    return _numFramesOfSimulatedLatency;
}

double EngineConfig::frameRate()
{
    return _frameRate;
}

uint8_t EngineConfig::timeScale()
{
    return _timeScale;
}

EngineConfig::EngineConfig(std::string configFilePath) :
_config(ConfigLoader::initWithJSONFile(configFilePath)),
_glfwSwapInterval(_config.getUInt("glfwSwapInterval", 1)),
_framesPerSecond(_config.getUInt("framesPerSecond", 60)),
_filePathEngineAssets(_config.getString("filePathEngineAssets")),
_filePathEntityLayoutManager(_config.getString("filePathEntityLayoutManager")),
_filePathEntityManager(_config.getString("filePathEntityManager")),
_glfwLoggingEnabled(_config.getBool("glfwLoggingEnabled", false)),
_fileLoggingEnabled(_config.getBool("fileLoggingEnabled", false)),
_inputLoggingEnabled(_config.getBool("inputLoggingEnabled", false)),
_networkLoggingEnabled(_config.getBool("networkLoggingEnabled", false)),
_physicsLoggingEnabled(_config.getBool("physicsLoggingEnabled", false)),
_soundsDisabled(_config.getBool("soundsDisabled", false)),
_musicDisabled(_config.getBool("musicDisabled", false)),
_maxTextInputLength(_config.getUInt("maxTextInputLength", 16)),
_maxNumSoundsToPlayPerFrame(_config.getUInt("maxNumSoundsToPlayPerFrame", 3)),
_clientPortHost(_config.getUInt("clientPortHost", 1337)),
_clientPortJoin(_config.getUInt("clientPortJoin", 1338)),
_serverPort(_config.getUInt("serverPort", 9999)),
_maxNumMoves(_config.getUInt("maxNumMoves", 7)),
_maxNumPacketsToProcessPerFrame(_config.getUInt("maxNumPacketsToProcessPerFrame", 4)),
_numFramesOfSimulatedLatency(_config.getUInt("numFramesOfSimulatedLatency", 0)),
_frameRate(1.0 / _framesPerSecond),
_timeScale(_framesPerSecond == 120 ? 4 : _framesPerSecond == 60 ? 2 : _framesPerSecond == 30 ? 1 : 0)
{
    assert(_timeScale == 4 || _timeScale == 2 || _timeScale == 1);
}

EngineConfig::~EngineConfig()
{
    // Empty
}
