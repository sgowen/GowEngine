//
//  FileAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/20/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <fstream>

FileData FileAssetHandler::loadAsset(std::string filePath)
{
    std::string resFilePath = RES_FILE_PATH(filePath);
    
    FILE* stream = OPEN_FILE(resFilePath, "r");
    if (stream == nullptr)
    {
        EmbeddedAssetHandler& eah = EmbeddedAssetHandler::getInstance();
        
        // File not found, so crash unless the asset is embedded
        assert(eah.isAssetEmbedded(filePath));
        
        return eah.loadAsset(filePath);
    }

    fseek(stream, 0, SEEK_END);

    long size = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    uint8_t* buffer = (uint8_t*) malloc(size);
    
    rewind(stream);
    fread(buffer, size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return FileData(size, buffer, nullptr);
}

void FileAssetHandler::unloadAsset(const FileData& fileData)
{
    if (fileData._fileHandle != nullptr)
    {
        // _fileHandle is only set for embedded assets
        EmbeddedAssetHandler& eah = EmbeddedAssetHandler::getInstance();
        eah.unloadAsset(fileData);
        return;
    }
    
    assert(fileData._data != nullptr);

    free((void*)fileData._data);
}
