//
//  AppleAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_APPLE

#include <fstream>

void AppleAssetHandler::create(void* bundleRootFilePath)
{
    assert(s_instance == NULL);
    
    assert(bundleRootFilePath != NULL);
    
    std::string* bbp = static_cast<std::string*>(bundleRootFilePath);
    
    s_instance = new AppleAssetHandler(*bbp);
}

void AppleAssetHandler::destroy()
{
    assert(s_instance != NULL);

    delete s_instance;
    s_instance = NULL;
}

AppleAssetHandler* AppleAssetHandler::s_instance = NULL;

FileData AppleAssetHandler::loadAsset(std::string filePath)
{
    std::string bundlePath = getBundleFilePath(filePath);

    FILE* stream = OPEN_FILE(bundlePath, "r");
    assert(stream != NULL);

    fseek(stream, 0, SEEK_END);

    long size = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    uint8_t* buffer = (uint8_t*) malloc(size);
    
    rewind(stream);
    fread(buffer, size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return FileData(size, buffer, NULL);
}

void AppleAssetHandler::unloadAsset(const FileData& fileData)
{
    assert(fileData._data != NULL);

    free((void *)fileData._data);
}

std::string AppleAssetHandler::getBundleFilePath(std::string filePath)
{
    return _bundleRootFilePath + filePath;
}

AppleAssetHandler::AppleAssetHandler(std::string bundleRootFilePath) : AssetHandler(),
_bundleRootFilePath(bundleRootFilePath)
{
    // Empty
}

#endif /* IS_APPLE */
