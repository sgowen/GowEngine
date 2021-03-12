//
//  AndroidAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"
#if IS_ANDROID

#include "AssetHandler.hpp"

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <assert.h>

class AndroidAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    static void create(void* jniEnv, void* assetManager);
    static void destroy();
    
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);

private:
    static AndroidAssetHandler* s_instance;
    
    static AndroidAssetHandler& getInstance()
    {
        assert(s_instance != NULL);
        return *s_instance;
    }

    AAssetManager* _assetManager;

    AndroidAssetHandler(JNIEnv *jniEnv, jobject assetManager);
    virtual ~AndroidAssetHandler() {}
    AndroidAssetHandler(const AndroidAssetHandler&);
    AndroidAssetHandler& operator=(const AndroidAssetHandler&);
};

#endif /* IS_ANDROID */
