//
//  AndroidAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidAssetHandler.hpp"

#include "JNIAndroidAssetHandler.hpp"

FileData AndroidAssetHandler::loadAsset(const char* filePath)
{
    return JNIAndroidAssetHandler::getInstance()->loadAsset(filePath);
}

void AndroidAssetHandler::releaseAsset(const FileData& fileData)
{
    JNIAndroidAssetHandler::getInstance()->releaseAsset(fileData);
}
