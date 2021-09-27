//
//  FileUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "FileUtil.hpp"

#include <GowEngine/BuildMacros.hpp>

FILE* FileUtil::openFile(std::string filePath, std::string mode)
{
    FILE *ret;

#if IS_WINDOWS
    errno_t err;
    if ((err = fopen_s(&ret, filePath.c_str(), mode.c_str())) != 0)
#else
    if ((ret = fopen(filePath.c_str(), mode.c_str())) == nullptr)
#endif
    {
        return nullptr;
    }

    return ret;
}
