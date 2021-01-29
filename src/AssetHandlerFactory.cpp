//
//  AssetHandlerFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AssetHandlerFactory.hpp"

#include "PlatformMacros.hpp"

#if IS_APPLE
    #include "apple/AppleAssetHandler.hpp"
#elif IS_ANDROID
    #include "android/AndroidAssetHandler.hpp"
#elif IS_LINUX
    #include "linux/LinuxAssetHandler.hpp"
#elif IS_WINDOWS
    #include "windows/WindowsAssetHandler.hpp"
#else
    #include "NullAssetHandler.hpp"
#endif

#include <assert.h>
#include <stdlib.h>

AssetHandler* AssetHandlerFactory::create()
{
#if IS_APPLE
    return new AppleAssetHandler();
#elif IS_ANDROID
    return new AndroidAssetHandler();
#elif IS_LINUX
    return new LinuxAssetHandler();
#elif IS_WINDOWS
    return new WindowsAssetHandler();
#else
    return new NullAssetHandler();
#endif
}

void AssetHandlerFactory::destroy(AssetHandler* ah)
{
    assert(ah != NULL);

    delete ah;
}
