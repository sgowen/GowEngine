//
//  AssetHandlerFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

AssetHandler& AssetHandlerFactory::getInstance()
{
#if IS_APPLE
    return AppleAssetHandler::getInstance();
#elif IS_ANDROID
    return LinuxAssetHandler::getInstance();
#elif IS_LINUX
    return LinuxAssetHandler::getInstance();
#else
    #error Need to add AssetHandler implementation
#endif
}
