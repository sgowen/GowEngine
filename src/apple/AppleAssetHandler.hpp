//
//  AppleAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AssetHandler.hpp"

class AppleAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(const char* filePath);
    virtual void releaseAsset(const FileData& fileData);
    
private:
    AppleAssetHandler() : AssetHandler() {}
    virtual ~AppleAssetHandler() {}
};
