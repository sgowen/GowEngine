//
//  FileCompatUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/20/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

#include <GowEngine/BuildConstants.hpp>
#if IS_ANDROID
#include <android/native_activity.h>
#endif

class FileCompatUtil
{
public:
#if IS_ANDROID
    static ANativeActivity* androidActivity;
#endif
    
    static std::string resourceFilePath(std::string filePath);
    
    static FILE* openFile(std::string filePath, std::string mode);
};
