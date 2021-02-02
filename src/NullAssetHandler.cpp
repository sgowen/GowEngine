//
//  NullAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullAssetHandler.hpp"

#include <stddef.h>

FileData NullAssetHandler::loadAsset(const char* filePath)
{
    return FileData(0, NULL, NULL);
}
