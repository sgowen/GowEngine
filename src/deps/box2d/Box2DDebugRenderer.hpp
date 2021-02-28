//
//  Box2DDebugRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "CircleBatcher.hpp"
#include "RektangleBatcher.hpp"
#include "TriangleBatcher.hpp"

#include <box2d/b2_draw.h>

struct Shader;
class b2World;

class Box2DDebugRenderer : public b2Draw
{
public:
    Box2DDebugRenderer(uint32_t maxBatchSize) :
    _circleBatcher(maxBatchSize),
    _fillRektangleBatcher(maxBatchSize, true),
    _boundsRektangleBatcher(maxBatchSize, false),
    _fillTriangleBatcher(maxBatchSize, true),
    _boundsTriangleBatcher(maxBatchSize, false),
    _shader(NULL),
    _matrix(NULL)
    {
        // Empty
    }
    virtual ~Box2DDebugRenderer() {}
    
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
    virtual void DrawTransform(const b2Transform& xf) {}
    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void render(Shader* shader, mat4* matrix, b2World* world);
    
private:
    CircleBatcher _circleBatcher;
    RektangleBatcher _fillRektangleBatcher;
    RektangleBatcher _boundsRektangleBatcher;
    TriangleBatcher _fillTriangleBatcher;
    TriangleBatcher _boundsTriangleBatcher;
    Shader* _shader;
    mat4* _matrix;
};
