//
//  JNIAndroidAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "FileData.hpp"

#include <jni.h>
#include <android/asset_manager_jni.h>

class JNIAndroidAssetHandler
{
public:
    static void create(JNIEnv *env, jobject assetManager);
    static JNIAndroidAssetHandler* getInstance();
    static void destroy();
    
    FileData loadAsset(const char* filePath);
    void releaseAsset(const FileData& fileData);

private:
    static JNIAndroidAssetHandler* s_instance;

    AAssetManager* _assetManager;

    JNIAndroidAssetHandler(JNIEnv *env, jobject assetManager);
    ~JNIAndroidAssetHandler();
    JNIAndroidAssetHandler(const JNIAndroidAssetHandler&);
    JNIAndroidAssetHandler& operator=(const JNIAndroidAssetHandler&);
};
