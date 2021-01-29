//
//  AppleAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleAssetHandler.hpp"

#include "NSAppleAssetHandler.hpp"

FileData AppleAssetHandler::loadAsset(const char* filePath)
{
    return appleLoadAsset(filePath);
}

void AppleAssetHandler::releaseAsset(const FileData& fileData)
{
    appleReleaseAsset(fileData);
}
