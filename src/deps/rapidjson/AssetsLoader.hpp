//
//  AssetsLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct Assets;

class AssetsLoader
{
public:
	static void initWithJSONFile(Assets& assets, std::string filePath);
    static void initWithJSON(Assets& assets, const char* json);

private:
    AssetsLoader();
    ~AssetsLoader();
    AssetsLoader(const AssetsLoader&);
    AssetsLoader& operator=(const AssetsLoader&);
};
