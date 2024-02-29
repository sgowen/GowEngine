//
//  FileCompatUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/20/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include "file_compat.h"

#if IS_ANDROID
#define FILE_COMPAT_ANDROID_ACTIVITY FileCompatUtil::androidActivity

ANativeActivity* FileCompatUtil::androidActivity = nullptr;
#endif

std::string FileCompatUtil::resourceFilePath(std::string filePath)
{
#if IS_APPLE
    char resourceDir[PATH_MAX];
    fc_resdir(resourceDir, PATH_MAX);
    
    return std::string(resourceDir) + filePath;
#else
    return filePath;
#endif
}

std::string FileCompatUtil::cacheDir()
{
    char cacheDir[PATH_MAX];
    fc_cachedir("gowengine", cacheDir, PATH_MAX);
    
    return std::string(cacheDir);
}

FILE* FileCompatUtil::openFile(std::string filePath, std::string mode)
{
    FILE* ret;

    if ((ret = fopen(filePath.c_str(), mode.c_str())) == nullptr)
    {
        return nullptr;
    }

    return ret;
}
