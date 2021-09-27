//
//  WindowsAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_WINDOWS

#include "AssetHandler.hpp"

class WindowsAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
private:
    static WindowsAssetHandler& getInstance()
    {
        static WindowsAssetHandler ret = WindowsAssetHandler();
        return ret;
    }
    
    WindowsAssetHandler() : AssetHandler() {}
    virtual ~WindowsAssetHandler() {}
};

#endif /* IS_WINDOWS */
