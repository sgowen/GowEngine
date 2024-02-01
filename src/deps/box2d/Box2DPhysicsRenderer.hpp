//
//  Box2DPhysicsRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/CircleBatcher.hpp"
#include "core/opengl/RektangleBatcher.hpp"
#include "core/opengl/TriangleBatcher.hpp"

#include <box2d/b2_draw.h>

class Box2DPhysicsWorld;
struct Shader;

class Box2DPhysicsRenderer : public b2Draw
{
public:
    Box2DPhysicsRenderer(uint32_t maxBatchSize) :
    _circleBatcher(maxBatchSize),
    _fillRektangleBatcher(maxBatchSize, true),
    _boundsRektangleBatcher(maxBatchSize, false),
    _fillTriangleBatcher(maxBatchSize, true),
    _boundsTriangleBatcher(maxBatchSize, false),
    _matrix(nullptr),
    _shader(nullptr)
    {
        // Empty
    }
    virtual ~Box2DPhysicsRenderer() {}
    
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
    virtual void DrawTransform(const b2Transform& xf) {}
    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void render(Box2DPhysicsWorld* world, mat4* matrix, Shader* shader);
    
private:
    CircleBatcher _circleBatcher;
    RektangleBatcher _fillRektangleBatcher;
    RektangleBatcher _boundsRektangleBatcher;
    TriangleBatcher _fillTriangleBatcher;
    TriangleBatcher _boundsTriangleBatcher;
    mat4* _matrix;
    Shader* _shader;
};
