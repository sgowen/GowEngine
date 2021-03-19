//
//  AndroidAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_ANDROID

#include <android/asset_manager.h>

void AndroidAssetHandler::create(void* jniEnv, void* assetManager)
{
    assert(s_instance == NULL);
    
    assert(jniEnv != NULL);
    assert(assetManager != NULL);
    
    s_instance = new AndroidAssetHandler(static_cast<JNIEnv*>(jniEnv), static_cast<jobject>(assetManager));
}

void AndroidAssetHandler::destroy()
{
    assert(s_instance != NULL);

    delete s_instance;
    s_instance = NULL;
}

AndroidAssetHandler* AndroidAssetHandler::s_instance = NULL;

FileData AndroidAssetHandler::loadAsset(std::string filePath)
{
    AAsset *asset = AAssetManager_open(_assetManager, filePath.c_str(), AASSET_MODE_STREAMING);
    if (asset == NULL)
    {
        LOG("Unable to load %s from AssetManager", filePath.c_str());
    }
    assert(asset != NULL);
    
    return FileData(AAsset_getLength(asset), static_cast<const uint8_t*>(AAsset_getBuffer(asset)), asset);
}

void AndroidAssetHandler::unloadAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != NULL);
    
    AAsset_close((AAsset*)fileData._fileHandle);
}

AndroidAssetHandler::AndroidAssetHandler(JNIEnv *jniEnv, jobject assetManager) : AssetHandler()
{
    assert(jniEnv != NULL);
    
    _assetManager = AAssetManager_fromJava(jniEnv, assetManager);
    assert(_assetManager != NULL);
}

#endif /* IS_ANDROID */
