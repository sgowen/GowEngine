//
//  EmbeddedAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/16/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>

#include "AssetHandler.hpp"

class EmbeddedAssetHandler : public AssetHandler
{
public:
    static EmbeddedAssetHandler& getInstance()
    {
        static EmbeddedAssetHandler ret = EmbeddedAssetHandler();
        return ret;
    }
    
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
    bool isAssetEmbedded(std::string filePath);
    
private:
    EmbeddedAssetHandler() : AssetHandler() {}
    virtual ~EmbeddedAssetHandler() {}
    EmbeddedAssetHandler(const EmbeddedAssetHandler&);
    EmbeddedAssetHandler& operator=(const EmbeddedAssetHandler&);
};
