//
//  JNIAndroidAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "JNIAndroidAssetHandler.hpp"

#include "StringUtil.hpp"

#include <android/asset_manager.h>

#include <assert.h>

void JNIAndroidAssetHandler::create(void *data1, void* data2)
{
    assert(s_instance == NULL);
    
    assert(data1 != NULL);
    assert(data2 != NULL);
    
    s_instance = new JNIAndroidAssetHandler(static_cast<JNIEnv*>(data1), static_cast<jobject>(data2));
}

JNIAndroidAssetHandler* JNIAndroidAssetHandler::getInstance()
{
    return s_instance;
}

void JNIAndroidAssetHandler::destroy()
{
    assert(s_instance != NULL);

    delete s_instance;
    s_instance = NULL;
}

JNIAndroidAssetHandler* JNIAndroidAssetHandler::s_instance = NULL;

FileData JNIAndroidAssetHandler::loadAsset(const char* filePath)
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

void JNIAndroidAssetHandler::releaseAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != NULL);
    
    AAsset_close((AAsset*)fileData._fileHandle);
}

JNIAndroidAssetHandler::JNIAndroidAssetHandler(JNIEnv *env, jobject assetManager)
{
    assert(env != NULL);
    
    _assetManager = AAssetManager_fromJava(env, assetManager);
    assert(_assetManager != NULL);
}
