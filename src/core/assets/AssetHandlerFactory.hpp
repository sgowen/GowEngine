//
//  AssetHandlerFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class AssetHandler;

class AssetHandlerFactory
{
public:
    static AssetHandler& getInstance();

private:
    AssetHandlerFactory();
    ~AssetHandlerFactory();
    AssetHandlerFactory(const AssetHandlerFactory&);
    AssetHandlerFactory& operator=(const AssetHandlerFactory&);
};
