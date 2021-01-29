//
//  AssetHandlerFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class AssetHandler;

class AssetHandlerFactory
{
public:
    static AssetHandler* create();
    static void destroy(AssetHandler* ah);
    
private:
    AssetHandlerFactory();
    ~AssetHandlerFactory();
    AssetHandlerFactory(const AssetHandlerFactory&);
    AssetHandlerFactory& operator=(const AssetHandlerFactory&);
};
