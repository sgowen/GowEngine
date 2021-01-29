//
//  NullAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullAssetHandler.hpp"

NullAssetHandler::NullAssetHandler() : AssetHandler()
{
    // Empty
}

NullAssetHandler::~NullAssetHandler()
{
    // Empty
}

FileData NullAssetHandler::loadAsset(const char* filePath)
{
    return FileData(0, (const char*)0, (const void*)0);
}

void NullAssetHandler::releaseAsset(const FileData& fileData)
{
    // Empty
}
