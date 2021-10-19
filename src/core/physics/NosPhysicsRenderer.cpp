//
//  NosPhysicsRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void NosPhysicsRenderer::createDeviceDependentResources()
{
    _circleBatcher.createDeviceDependentResources();
    _fillRektangleBatcher.createDeviceDependentResources();
    _boundsRektangleBatcher.createDeviceDependentResources();
    _fillTriangleBatcher.createDeviceDependentResources();
    _boundsTriangleBatcher.createDeviceDependentResources();
}

void NosPhysicsRenderer::destroyDeviceDependentResources()
{
    _circleBatcher.destroyDeviceDependentResources();
    _fillRektangleBatcher.destroyDeviceDependentResources();
    _boundsRektangleBatcher.destroyDeviceDependentResources();
    _fillTriangleBatcher.destroyDeviceDependentResources();
    _boundsTriangleBatcher.destroyDeviceDependentResources();
}

void NosPhysicsRenderer::render(NosPhysicsWorld& world, mat4* matrix, Shader* shader)
{
    assert(shader != nullptr);
    assert(matrix != nullptr);
    
    _matrix = matrix;
    _shader = shader;
    
    // TODO
}
