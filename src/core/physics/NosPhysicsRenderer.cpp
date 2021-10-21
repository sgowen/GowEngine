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

void NosPhysicsRenderer::render(NosPhysicsWorld* world, mat4* matrix, Shader* shader)
{
    assert(world  != nullptr);
    assert(shader != nullptr);
    assert(matrix != nullptr);
    
    _matrix = matrix;
    _shader = shader;
    
    Color c = Color(0.5f, 0.9f, 0.5f, 0.3f);
    
    _fillRektangleBatcher.begin();
    for (Entity* e : world->getPlayers())
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        std::vector<Bounds>& bounds = epc->bounds();
        
        for (Bounds& b : bounds)
        {
            _fillRektangleBatcher.addRektangle(b._boundingBox);
        }
    }
    for (Entity* e : world->getNetworkEntities())
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        std::vector<Bounds>& bounds = epc->bounds();
        
        for (Bounds& b : bounds)
        {
            _fillRektangleBatcher.addRektangle(b._boundingBox);
        }
    }
    for (Entity* e : world->getStaticEntities())
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        std::vector<Bounds>& bounds = epc->bounds();
        
        for (Bounds& b : bounds)
        {
            _fillRektangleBatcher.addRektangle(b._boundingBox);
        }
    }
    _fillRektangleBatcher.end(*_shader, *_matrix, c);
    
    // TODO ?
}
