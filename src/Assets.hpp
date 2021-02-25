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

class Assets
{
    friend class AssetManager;
    
public:
	void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    TextureRegion* findTextureRegion(std::string key, uint16_t stateTime = 0);
    std::vector<ShaderDescriptor>& getShaderDescriptors();
    std::vector<SoundDescriptor>& getSoundDescriptors();
    std::vector<TextureDescriptor>& getTextureDescriptors();

private:
    std::vector<ShaderDescriptor> _shaderDescriptors;
    std::vector<SoundDescriptor> _soundDescriptors;
    std::vector<TextureDescriptor> _textureDescriptors;
};
