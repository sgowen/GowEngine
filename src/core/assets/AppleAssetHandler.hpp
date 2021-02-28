//
//  AppleAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"
#if IS_APPLE

#include "AssetHandler.hpp"

class AppleAssetHandler : public AssetHandler
{
    friend class AssetHandlerFactory;
    friend class ObjectALSound;
    
public:
    static void create(void* bundleRootFilePath);
    static void destroy();
    
    virtual FileData loadAsset(std::string filePath);
    virtual void unloadAsset(const FileData& fileData);
    
    std::string getBundleFilePath(std::string filePath);
    
private:
    static AppleAssetHandler* s_instance;
    
    static AppleAssetHandler& getInstance()
    {
        assert(s_instance != NULL);
        return *s_instance;
    }
    
    std::string _bundleRootFilePath;
    
    AppleAssetHandler(std::string bundleRootFilePath);
    virtual ~AppleAssetHandler() {}
};

#endif /* IS_APPLE */
