//
//  LinuxAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_LINUX || IS_APPLE

#include "AssetHandler.hpp"

class LinuxAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
protected:
    LinuxAssetHandler() : AssetHandler() {}
    virtual ~LinuxAssetHandler() {}
    
private:
    static LinuxAssetHandler& getInstance()
    {
        static LinuxAssetHandler ret = LinuxAssetHandler();
        return ret;
    }
};

#endif /* IS_LINUX || IS_APPLE */
