//
//  AssetHandlerFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"

#include "AndroidAssetHandler.hpp"
#include "AppleAssetHandler.hpp"
#include "LinuxAssetHandler.hpp"
#include "WindowsAssetHandler.hpp"

#include <cassert>
#include <stdlib.h>

#define ASSET_HANDLER AssetHandlerFactory::getInstance()

class AssetHandlerFactory
{
public:
    static AssetHandler& getInstance()
    {
#if IS_APPLE
        return AppleAssetHandler::getInstance();
#elif IS_ANDROID
        return AndroidAssetHandler::getInstance();
#elif IS_LINUX
        return LinuxAssetHandler::getInstance();
#elif IS_WINDOWS
        return WindowsAssetHandler::getInstance();
#else
        #error Need to add AssetHandler implementation
#endif
    }

private:
    AssetHandlerFactory();
    ~AssetHandlerFactory();
    AssetHandlerFactory(const AssetHandlerFactory&);
    AssetHandlerFactory& operator=(const AssetHandlerFactory&);
};
