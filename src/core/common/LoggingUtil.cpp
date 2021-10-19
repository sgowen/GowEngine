//
//  LoggingUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/18/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void LoggingUtil::setGlfwLoggingEnabled(bool isEnabled)
{
    _isGlfwLoggingEnabled = isEnabled;
}

bool LoggingUtil::isGlfwLoggingEnabled()
{
    return _isGlfwLoggingEnabled;
}

void LoggingUtil::setInputLoggingEnabled(bool isEnabled)
{
    _isInputLoggingEnabled = isEnabled;
}

bool LoggingUtil::isInputLoggingEnabled()
{
    return _isInputLoggingEnabled;
}

void LoggingUtil::setNetworkLoggingEnabled(bool isEnabled)
{
    _isNetworkLoggingEnabled = isEnabled;
}

bool LoggingUtil::isNetworkLoggingEnabled()
{
    return _isNetworkLoggingEnabled;
}

void LoggingUtil::setPhysicsLoggingEnabled(bool isEnabled)
{
    _isPhysicsLoggingEnabled = isEnabled;
}

bool LoggingUtil::isPhysicsLoggingEnabled()
{
    return _isPhysicsLoggingEnabled;
}

LoggingUtil::LoggingUtil() :
_isGlfwLoggingEnabled(false),
_isInputLoggingEnabled(false),
_isNetworkLoggingEnabled(false),
_isPhysicsLoggingEnabled(false)
{
    // Empty
}
