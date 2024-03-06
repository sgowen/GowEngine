//
//  NosPhysicsRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NosPhysicsRenderer::NosPhysicsRenderer(uint32_t maxBatchSize) :
_circleBatcher(maxBatchSize),
_fillRektangleBatcher(maxBatchSize, true),
_boundsRektangleBatcher(maxBatchSize, false),
_matrix(nullptr),
_shader(nullptr)
{
    // Empty
}

NosPhysicsRenderer::~NosPhysicsRenderer()
{
    // Empty
}

void NosPhysicsRenderer::createDeviceDependentResources()
{
    _circleBatcher.createDeviceDependentResources();
    _fillRektangleBatcher.createDeviceDependentResources();
    _boundsRektangleBatcher.createDeviceDependentResources();
}

void NosPhysicsRenderer::destroyDeviceDependentResources()
{
    _circleBatcher.destroyDeviceDependentResources();
    _fillRektangleBatcher.destroyDeviceDependentResources();
    _boundsRektangleBatcher.destroyDeviceDependentResources();
}

void NosPhysicsRenderer::render(NosPhysicsWorld* world, mat4* matrix, Shader* shader)
{
    assert(world  != nullptr);
    assert(shader != nullptr);
    assert(matrix != nullptr);
    
    _matrix = matrix;
    _shader = shader;
    
    {
        _fillRektangleBatcher.begin();
        for (Entity* e : world->getPlayers())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _fillRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        for (Entity* e : world->getDynamicEntities())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _fillRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        Color c = Color(0.5f, 0.9f, 0.5f, 0.4f);
        _fillRektangleBatcher.end(*_shader, *_matrix, c);
    }
    
    {
        _fillRektangleBatcher.begin();
        for (Entity* e : world->getStaticEntities())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _fillRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        Color c = Color(0.5f, 0.9f, 0.5f, 0.2f);
        _fillRektangleBatcher.end(*_shader, *_matrix, c);
    }
    
    {
        Color c = Color(0.9f, 0.5f, 0.5f, 0.3f);
        
        _boundsRektangleBatcher.begin();
        for (Entity* e : world->getPlayers())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _boundsRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        for (Entity* e : world->getDynamicEntities())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _boundsRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        for (Entity* e : world->getStaticEntities())
        {
            NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
            
            std::vector<Bounds>& bounds = epc->bounds();
            
            for (Bounds& b : bounds)
            {
                _boundsRektangleBatcher.addRektangle(b._boundingBox);
            }
        }
        _boundsRektangleBatcher.end(*_shader, *_matrix, c);
    }
}
