//
//  EngineConfig.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

#include "core/common/Config.hpp"

class EngineConfig
{
public:
    static void create(std::string configFilePath);
    static EngineConfig& getInstance();
    static void destroy();
    
    bool vsync();
    bool consoleLoggingEnabled();
    uint8_t framesPerSecond();
    std::string fileDirLogger();
    std::string filePathEngineAssets();
    std::string filePathEntityLayoutManager();
    std::string filePathEntityManager();
    bool glfwLoggingEnabled();
    bool fileLoggingEnabled();
    bool inputLoggingEnabled();
    bool networkLoggingEnabled();
    bool physicsLoggingEnabled();
    bool audioDisabled();
    bool musicDisabled();
    bool extrapolatePhysics();
    uint16_t clientPortHost();
    uint16_t clientPortJoin();
    uint16_t serverPort();
    uint8_t maxNumFramesOfRollback();
    uint8_t numFramesOfInputDelay();
    uint8_t numFramesOfSimulatedLatency();
    
    /// Derived Values
    double frameRate();
    
private:
    static EngineConfig* s_instance;
    
    Config _config;
    bool _vsync;
    bool _consoleLoggingEnabled;
    uint8_t _framesPerSecond;
    std::string _fileDirLogger;
    std::string _filePathEngineAssets;
    std::string _filePathEntityLayoutManager;
    std::string _filePathEntityManager;
    bool _glfwLoggingEnabled;
    bool _fileLoggingEnabled;
    bool _inputLoggingEnabled;
    bool _networkLoggingEnabled;
    bool _physicsLoggingEnabled;
    bool _audioDisabled;
    bool _musicDisabled;
    bool _extrapolatePhysics;
    uint16_t _clientPortHost;
    uint16_t _clientPortJoin;
    uint16_t _serverPort;
    uint8_t _maxNumFramesOfRollback;
    uint8_t _numFramesOfInputDelay;
    uint8_t _numFramesOfSimulatedLatency;
    
    /// Derived Values
    float _frameRate;
    
    EngineConfig(std::string configFilePath);
    ~EngineConfig();
    EngineConfig(const EngineConfig&);
    EngineConfig& operator=(const EngineConfig&);
};
