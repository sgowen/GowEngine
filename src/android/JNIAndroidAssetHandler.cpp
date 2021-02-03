//
//  JNIAndroidAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "JNIAndroidAssetHandler.hpp"

#include <android/asset_manager.h>

#include <assert.h>

void JNIAndroidAssetHandler::create(JNIEnv *env, jobject assetManager)
{
    assert(s_instance == NULL);

    s_instance = new JNIAndroidAssetHandler(env, assetManager);
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
    assert(asset != NULL);
    
    return FileData(AAsset_getLength(asset), static_cast<const uint8_t *>(AAsset_getBuffer(asset)), asset);
}

void JNIAndroidAssetHandler::releaseAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != NULL);
    
    AAsset_close((AAsset*)fileData._fileHandle);
}

JNIAndroidAssetHandler::JNIAndroidAssetHandler(JNIEnv *env, jobject assetManager)
{
    _assetManager = AAssetManager_fromJava(env, assetManager);
    assert(_assetManager != NULL);
}
