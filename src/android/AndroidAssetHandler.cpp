//
//  AndroidAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidAssetHandler.hpp"

#include "StringUtil.hpp"

#include <android/asset_manager.h>

void AndroidAssetHandler::create(void *data1, void* data2)
{
    assert(s_instance == NULL);
    
    assert(data1 != NULL);
    assert(data2 != NULL);
    
    s_instance = new AndroidAssetHandler(static_cast<JNIEnv*>(data1), static_cast<jobject>(data2));
}

void AndroidAssetHandler::destroy()
{
    assert(s_instance != NULL);

    delete s_instance;
    s_instance = NULL;
}

AndroidAssetHandler* AndroidAssetHandler::s_instance = NULL;

FileData AndroidAssetHandler::loadAsset(const char* filePath)
{
    assert(filePath != NULL);
    
    AAsset *asset = AAssetManager_open(_assetManager, filePath, AASSET_MODE_STREAMING);
    if (asset == NULL)
    {
        LOG("Unable to load %s from AssetManager", filePath);
    }
    assert(asset != NULL);
    
    return FileData(AAsset_getLength(asset), static_cast<const uint8_t*>(AAsset_getBuffer(asset)), asset);
}

void AndroidAssetHandler::unloadAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != NULL);
    
    AAsset_close((AAsset*)fileData._fileHandle);
}

AndroidAssetHandler::AndroidAssetHandler(JNIEnv *env, jobject assetManager) : AssetHandler() {}
{
    assert(env != NULL);
    
    _assetManager = AAssetManager_fromJava(env, assetManager);
    assert(_assetManager != NULL);
}
