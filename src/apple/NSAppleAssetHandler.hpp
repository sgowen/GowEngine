//
//  NSAppleAssetHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "FileData.hpp"

FileData appleLoadAsset(const char* filePath);
void appleReleaseAsset(const FileData& fileData);
const char * getBundlePath(const char* filePath);
