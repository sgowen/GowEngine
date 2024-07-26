//
//  EngineConfig.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EngineConfig* EngineConfig::s_instance = nullptr;

void EngineConfig::create(std::string filePathConfig)
{
    assert(s_instance == nullptr);
    
    s_instance = new EngineConfig(filePathConfig);
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

bool EngineConfig::fullScreen()
{
    return _fullScreen;
}

std::string EngineConfig::title()
{
    return _title;
}

std::string& EngineConfig::mode()
{
    return _mode;
}

std::string& EngineConfig::physicsEngine()
{
    return _physicsEngine;
}

bool EngineConfig::vsync()
{
    return _vsync;
}

bool EngineConfig::useSteamNetworking()
{
    return _useSteamNetworking;
}

std::string EngineConfig::versionName()
{
    return _versionName;
}

std::string EngineConfig::steamGameDir()
{
    return _steamGameDir;
}

std::string EngineConfig::steamProductName()
{
    return _steamProductName;
}

std::string EngineConfig::steamProductDescription()
{
    return _steamProductDescription;
}

bool EngineConfig::consoleLoggingEnabled()
{
    return _consoleLoggingEnabled;
}

uint8_t EngineConfig::framesPerSecond()
{
    return _framesPerSecond;
}

bool EngineConfig::glfwLoggingEnabled()
{
    return _glfwLoggingEnabled;
}

bool EngineConfig::logFileIO()
{
    return _logFileIO;
}

bool EngineConfig::logLua()
{
    return _logLua;
}

bool EngineConfig::logOpenGL()
{
    return _logOpenGL;
}

bool EngineConfig::logOpenAL()
{
    return _logOpenAL;
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

bool EngineConfig::audioDisabled()
{
    return _audioDisabled;
}

bool EngineConfig::musicDisabled()
{
    return _musicDisabled;
}

float EngineConfig::volume()
{
    return _volume;
}

bool EngineConfig::extrapolatePhysics()
{
    return _extrapolatePhysics;
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

uint16_t EngineConfig::mtuSize()
{
    return _mtuSize;
}

uint8_t EngineConfig::maxNumFramesOfRollback()
{
    return _maxNumFramesOfRollback;
}

uint8_t EngineConfig::numFramesOfInputDelay()
{
    return _numFramesOfInputDelay;
}

uint8_t EngineConfig::numFramesOfSimulatedLatency()
{
    return _numFramesOfSimulatedLatency;
}

double EngineConfig::frameRate()
{
    return _frameRate;
}

EngineConfig::EngineConfig(std::string filePathConfig)
{
    ConfigLoader::initWithJSONFile(_config, filePathConfig);
    
    _fullScreen = _config.getBool("fullScreen");
    _title = _config.getString("title");
    _mode = _config.getString("mode");
    _physicsEngine = _config.getBool("physicsEngine");
    _vsync = _config.getBool("vsync");
    _useSteamNetworking = _config.getBool("useSteamNetworking");
    _versionName = _config.getString("versionName");
    _steamGameDir = _config.getString("steamGameDir");
    _steamProductName = _config.getString("steamProductName");
    _steamProductDescription = _config.getString("steamProductDescription");
    _consoleLoggingEnabled = _config.getBool("consoleLoggingEnabled");
    _framesPerSecond = _config.getUInt("framesPerSecond");
    _glfwLoggingEnabled = _config.getBool("glfwLoggingEnabled");
    _logFileIO = _config.getBool("logFileIO");
    _logLua = _config.getBool("logLua");
    _logOpenGL = _config.getBool("logOpenGL");
    _logOpenAL = _config.getBool("logOpenAL");
    _fileLoggingEnabled = _config.getBool("fileLoggingEnabled");
    _inputLoggingEnabled = _config.getBool("inputLoggingEnabled");
    _networkLoggingEnabled = _config.getBool("networkLoggingEnabled");
    _physicsLoggingEnabled = _config.getBool("physicsLoggingEnabled");
    _audioDisabled = _config.getBool("audioDisabled");
    _musicDisabled = _config.getBool("musicDisabled");
    _volume = _config.getFloat("volume");
    _extrapolatePhysics = _config.getBool("extrapolatePhysics");
    _clientPortHost = _config.getUInt("clientPortHost");
    _clientPortJoin = _config.getUInt("clientPortJoin");
    _serverPort = _config.getUInt("serverPort");
    _mtuSize = _config.getUInt("mtuSize");
    _maxNumFramesOfRollback = _config.getUInt("maxNumFramesOfRollback");
    _numFramesOfInputDelay = _config.getUInt("numFramesOfInputDelay");
    _numFramesOfSimulatedLatency = _config.getUInt("numFramesOfSimulatedLatency");
    _frameRate = 1.0 / _framesPerSecond;
}

EngineConfig::~EngineConfig()
{
    // Empty
}
