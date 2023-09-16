//
//  AppleAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_APPLE

FileData AppleAssetHandler::loadAsset(std::string filePath)
{
    std::string bundlePath = getBundleFilePath(filePath);

    return LinuxAssetHandler::loadAsset(bundlePath);
}

#endif /* IS_APPLE */
