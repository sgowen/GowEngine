//
//  AppleAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_APPLE

#include "LinuxAssetHandler.hpp"

class AppleAssetHandler : public LinuxAssetHandler
{
    friend class AssetHandlerFactory;
    
public:
    virtual FileData loadAsset(std::string filePath);
    
private:
    static AppleAssetHandler& getInstance()
    {
        static AppleAssetHandler ret = AppleAssetHandler();
        return ret;
    }
    
    AppleAssetHandler() : LinuxAssetHandler() {}
    virtual ~AppleAssetHandler() {}
};

#endif /* IS_APPLE */
