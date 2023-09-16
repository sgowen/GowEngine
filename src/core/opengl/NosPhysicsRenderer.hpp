//
//  NosPhysicsRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/CircleBatcher.hpp"
#include "core/opengl/LineBatcher.hpp"
#include "core/opengl/RektangleBatcher.hpp"
#include "core/opengl/TriangleBatcher.hpp"

class NosPhysicsWorld;
struct Shader;

class NosPhysicsRenderer
{
public:
    NosPhysicsRenderer(uint32_t maxBatchSize);
    virtual ~NosPhysicsRenderer();
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void render(NosPhysicsWorld* world, mat4* matrix, Shader* shader);
    
private:
    CircleBatcher _circleBatcher;
    RektangleBatcher _fillRektangleBatcher;
    RektangleBatcher _boundsRektangleBatcher;
    mat4* _matrix;
    Shader* _shader;
};
