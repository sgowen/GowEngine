//
//  Logger.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/18/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <fstream>

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger ret = Logger();
        return ret;
    }
    
    static void log(std::string line);
    
    void initWithFile(const char* fileName);
    void closeFileStream();
    void log(const char* format, ...);

private:
    std::fstream _fileStream;
    
    Logger() {}
    ~Logger() {}
    Logger(const Logger&);
    Logger& operator=(const Logger&);
};
