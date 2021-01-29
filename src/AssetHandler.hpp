//
//  AssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "FileData.hpp"

class AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(const char* filePath) = 0;
    virtual void releaseAsset(const FileData& fileData) = 0;
    
protected:
    AssetHandler() {}
    virtual ~AssetHandler() {}
};
