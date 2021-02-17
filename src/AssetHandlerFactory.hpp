//
//  AssetHandlerFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

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
    #error Need to add AssetHandler implementation
#endif

#include <assert.h>
#include <stdlib.h>

class AssetHandlerFactory
{
public:
    static AssetHandler* create()
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
        #error Need to add AssetHandler implementation
#endif
    }
    
    static void destroy(AssetHandler* ah)
    {
        assert(ah != NULL);

        delete ah;
    }
    
private:
    AssetHandlerFactory();
    ~AssetHandlerFactory();
    AssetHandlerFactory(const AssetHandlerFactory&);
    AssetHandlerFactory& operator=(const AssetHandlerFactory&);
};
