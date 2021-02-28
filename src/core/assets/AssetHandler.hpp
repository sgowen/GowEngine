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
public:
    virtual FileData loadAsset(std::string filePath) = 0;
    virtual void unloadAsset(const FileData& fileData) = 0;
    
protected:
    AssetHandler() {}
    virtual ~AssetHandler() {}
};
