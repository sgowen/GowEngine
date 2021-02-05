//
//  EngineController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EngineController.hpp"

#if IS_ANDROID
    #include "android/JNIAndroidAssetHandler.hpp"
#endif

double EngineController::getFrameRate()
{
    // default is 60 FPS
    return (1.0 / 60.0);
}
    
EngineController::EngineController(void* data1, void* data2)
{
#if IS_ANDROID
    JNIAndroidAssetHandler::create(data1, data2);
#endif
}

EngineController::~EngineController()
{
#if IS_ANDROID
    JNIAndroidAssetHandler::destroy();
#endif
}
