//
//  AssetsLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Assets.hpp"

class AssetsLoader
{
public:
	static Assets initWithJSONFile(std::string filePath);
    static Assets initWithJSON(const char* json);

private:
    AssetsLoader();
    ~AssetsLoader();
    AssetsLoader(const AssetsLoader&);
    AssetsLoader& operator=(const AssetsLoader&);
};
