//
//  FileAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/20/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AssetHandler.hpp"

class FileAssetHandler : public AssetHandler
{
public:
    static FileAssetHandler& getInstance()
    {
        static FileAssetHandler ret = FileAssetHandler();
        return ret;
    }
    
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
protected:
    FileAssetHandler() : AssetHandler() {}
    virtual ~FileAssetHandler() {}
};
