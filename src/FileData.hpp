//
//  FileData.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct FileData
{
    const long _length;
    const char* _data;
    const void* _fileHandle;
    
    FileData(const long dataLength, const char* data, const void* fileHandle) :
    _length(dataLength),
    _data(data),
    _fileHandle(fileHandle)
    {
        // Empty
    }

    FileData(const FileData& other) :
    _length(other._length),
    _data(other._data),
    _fileHandle(other._fileHandle)
    {
        // Empty
    }
};
