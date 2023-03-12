//
//  AppleAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/BuildConstants.hpp>
#if IS_APPLE

#include <GowEngine/GowEngine.hpp>

FileData AppleAssetHandler::loadAsset(std::string filePath)
{
    std::string bundlePath = getBundleFilePath(filePath);

    return LinuxAssetHandler::loadAsset(bundlePath);
}

#endif /* IS_APPLE */
