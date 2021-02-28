//
//  PlatformMacros.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define PLATFORM_IOS 1
#define PLATFORM_MACOS 2
#define PLATFORM_ANDROID 3
#define PLATFORM_LINUX 4
#define PLATFORM_WINDOWS 5

#if defined __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define PLATFORM PLATFORM_IOS
    #elif TARGET_OS_OSX
        #define PLATFORM PLATFORM_MACOS
    #endif
#elif defined __ANDROID__
    #define PLATFORM PLATFORM_ANDROID
#elif defined __linux__
    #define PLATFORM PLATFORM_LINUX
#elif defined(WIN32) || defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#endif

#define IS_IOS PLATFORM == PLATFORM_IOS
#define IS_MACOS PLATFORM == PLATFORM_MACOS
#define IS_ANDROID PLATFORM == PLATFORM_ANDROID
#define IS_LINUX PLATFORM == PLATFORM_LINUX
#define IS_WINDOWS PLATFORM == PLATFORM_WINDOWS

#define IS_DESKTOP IS_MACOS || IS_LINUX || IS_WINDOWS
#define IS_MOBILE IS_IOS || IS_ANDROID
#define IS_APPLE IS_IOS || IS_MACOS

#if !defined(_DEBUG) && !defined(DEBUG)
    #define IS_RELEASE 1
#else
    #define IS_RELEASE 0
#endif

#if defined(_DEBUG) || defined(DEBUG)
    #define IS_DEBUG 1
#else
    #define IS_DEBUG 0
#endif
