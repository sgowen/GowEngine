//
//  TextureLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct Texture;

class TextureLoader
{
public:
    void loadData(Texture& t);
    void freeData(Texture& t);
};
