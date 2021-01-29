//
//  Box2DDebugRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Box2DDebugRenderer.hpp"

#include "Shader.hpp"
#include <box2d/box2d.h>

#include "Shader.hpp"
#include "Color.hpp"

Box2DDebugRenderer::Box2DDebugRenderer(int maxBatchSize) :
_fillPolygonBatcher(maxBatchSize, true),
_boundsPolygonBatcher(maxBatchSize, false),
_circleBatcher(maxBatchSize),
_shader(NULL),
_matrix(NULL)
{
    // Empty
}

void Box2DDebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    Color c = Color(color.r, color.g, color.b, 0.3f);
    _boundsPolygonBatcher.begin();
    _boundsPolygonBatcher.addPolygon(vertices, vertexCount);
    _boundsPolygonBatcher.end(*_shader, *_matrix, c);
}

void Box2DDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    {
        Color c = Color(color.r, color.g, color.b, 0.3f);
        _fillPolygonBatcher.begin();
        _fillPolygonBatcher.addPolygon(vertices, vertexCount);
        _fillPolygonBatcher.end(*_shader, *_matrix, c);
    }

    {
        Color c = Color(1, 0, 0, 0.3f);
        _boundsPolygonBatcher.begin();
        _boundsPolygonBatcher.addPolygon(vertices, vertexCount);
        _boundsPolygonBatcher.end(*_shader, *_matrix, c);
    }
}

void Box2DDebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    Color c = Color(color.r, color.g, color.b, 0.3f);
    _circleBatcher.begin();
    _circleBatcher.addCircle(center.x, center.y, radius);
    _circleBatcher.end(*_shader, *_matrix, c);
}

void Box2DDebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    Color c = Color(color.r, color.g, color.b, 0.3f);
    _circleBatcher.begin();
    _circleBatcher.addCircle(center.x, center.y, radius);
    _circleBatcher.end(*_shader, *_matrix, c);
}

void Box2DDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    // TODO
}

void Box2DDebugRenderer::DrawTransform(const b2Transform& xf)
{
    // TODO
}

void Box2DDebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    // TODO
}

void Box2DDebugRenderer::createDeviceDependentResources()
{
    _fillPolygonBatcher.createDeviceDependentResources();
    _boundsPolygonBatcher.createDeviceDependentResources();
    _circleBatcher.createDeviceDependentResources();
}

void Box2DDebugRenderer::releaseDeviceDependentResources()
{
    _fillPolygonBatcher.releaseDeviceDependentResources();
    _boundsPolygonBatcher.releaseDeviceDependentResources();
    _circleBatcher.releaseDeviceDependentResources();
}

void Box2DDebugRenderer::render(Shader* shader, mat4* matrix, b2World* world)
{
    assert(shader != NULL);
    assert(matrix != NULL);
    assert(world != NULL);
    
    _shader = shader;
    _matrix = matrix;
    
    world->SetDebugDraw(this);
    AppendFlags(e_shapeBit);
    world->DebugDraw();
}
