//
//  LinuxAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"
#if IS_LINUX

#include "AssetHandler.hpp"

class LinuxAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
private:
    static LinuxAssetHandler& getInstance()
    {
        static LinuxAssetHandler ret = LinuxAssetHandler();
        return ret;
    }
    
    LinuxAssetHandler() : AssetHandler() {}
    virtual ~LinuxAssetHandler() {}
};

#endif /* IS_LINUX */
