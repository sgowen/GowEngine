//
//  Assets.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderDescriptor.hpp"
#include "SoundDescriptor.hpp"
#include "TextureDescriptor.hpp"

#include <vector>

#define ASSETS Assets::getInstance()

class Assets
{
public:
	static Assets& getInstance();
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    TextureRegion& findTextureRegion(std::string key, uint16_t stateTime);
    TextureDescriptor& findTextureDescriptor(std::string key);
    std::vector<ShaderDescriptor>& getShaderDescriptors();
    std::vector<SoundDescriptor>& getSoundDescriptors();
    std::vector<TextureDescriptor>& getTextureDescriptors();

private:
    std::vector<ShaderDescriptor> _shaderDescriptors;
    std::vector<SoundDescriptor> _soundDescriptors;
    std::vector<TextureDescriptor> _textureDescriptors;
    
    Assets();
    ~Assets();
    Assets(const Assets&);
    Assets& operator=(const Assets&);
};
