//
//  LoggingUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/18/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define LOGGING_UTIL LoggingUtil::getInstance()
#define IS_GLFW_LOGGING_ENABLED LOGGING_UTIL.isGlfwLoggingEnabled
#define IS_INPUT_LOGGING_ENABLED LOGGING_UTIL.isInputLoggingEnabled
#define IS_NETWORK_LOGGING_ENABLED LOGGING_UTIL.isNetworkLoggingEnabled
#define IS_PHYSICS_LOGGING_ENABLED LOGGING_UTIL.isPhysicsLoggingEnabled

class LoggingUtil
{
public:
    static LoggingUtil& getInstance()
    {
        static LoggingUtil ret = LoggingUtil();
        return ret;
    }
    
    void setGlfwLoggingEnabled(bool isEnabled);
    bool isGlfwLoggingEnabled();
    void setInputLoggingEnabled(bool isEnabled);
    bool isInputLoggingEnabled();
    void setNetworkLoggingEnabled(bool isEnabled);
    bool isNetworkLoggingEnabled();
    void setPhysicsLoggingEnabled(bool isEnabled);
    bool isPhysicsLoggingEnabled();
    
private:
    bool _isGlfwLoggingEnabled;
    bool _isInputLoggingEnabled;
    bool _isNetworkLoggingEnabled;
    bool _isPhysicsLoggingEnabled;
    
    LoggingUtil();
    ~LoggingUtil() {}
    LoggingUtil(const LoggingUtil&);
    LoggingUtil& operator=(const LoggingUtil&);
};
