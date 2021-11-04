//
//  FileUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

class FileUtil
{
public:
    static FILE* openFile(std::string filePath, std::string mode);
};
