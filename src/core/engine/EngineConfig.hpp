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
    
    uint8_t glfwSwapInterval();
    uint8_t framesPerSecond();
    std::string filePathEngineAssets();
    std::string filePathEntityLayoutManager();
    std::string filePathEntityManager();
    bool glfwLoggingEnabled();
    bool inputLoggingEnabled();
    bool networkLoggingEnabled();
    bool physicsLoggingEnabled();
    bool soundsDisabled();
    bool musicDisabled();
    uint8_t maxNumPlayers();
    uint8_t maxTextInputLength();
    uint8_t maxNumSoundsToPlayPerFrame();
    uint16_t clientPortHost();
    uint16_t clientPortJoin();
    uint16_t serverPort();
    uint8_t maxNumMoves();
    uint8_t maxNumPacketsToProcessPerFrame();
    uint8_t numFramesOfSimulatedLatency();
    
    /// Derived Values
    float frameRate();
    uint8_t timeScale();
    
private:
    static EngineConfig* s_instance;
    
    Config _config;
    uint8_t _glfwSwapInterval;
    uint8_t _framesPerSecond;
    std::string _filePathEngineAssets;
    std::string _filePathEntityLayoutManager;
    std::string _filePathEntityManager;
    bool _glfwLoggingEnabled;
    bool _inputLoggingEnabled;
    bool _networkLoggingEnabled;
    bool _physicsLoggingEnabled;
    bool _soundsDisabled;
    bool _musicDisabled;
    uint8_t _maxNumPlayers;
    uint8_t _maxTextInputLength;
    uint8_t _maxNumSoundsToPlayPerFrame;
    uint16_t _clientPortHost;
    uint16_t _clientPortJoin;
    uint16_t _serverPort;
    uint8_t _maxNumMoves;
    uint8_t _maxNumPacketsToProcessPerFrame;
    uint8_t _numFramesOfSimulatedLatency;
    
    /// Derived Values
    float _frameRate;
    uint8_t _timeScale;
    
    EngineConfig(std::string configFilePath);
    ~EngineConfig();
    EngineConfig(const EngineConfig&);
    EngineConfig& operator=(const EngineConfig&);
};
