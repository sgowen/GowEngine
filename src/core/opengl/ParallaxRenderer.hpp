//
//  ParallaxRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/24/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class ParallaxRenderer
{
public:
    ParallaxRenderer();
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    
    void renderBackgroundLayers();
    void renderForegroundLayers();
};
