//
//  TextureLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct Texture;

class TextureLoader
{
public:
    TextureLoader() {}
    
    void loadTextureData(Texture& t);
    void releaseTextureData(Texture& t);
    void loadTexture(Texture& t);
    void unloadTexture(Texture& t);
};
