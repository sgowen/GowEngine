//
//  Box2DDebugRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

void Box2DDebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    Color c = Color(color.r, color.g, color.b, 0.3f);
    
    _boundsRektangleBatcher.begin();
    _boundsTriangleBatcher.begin();
    if (vertexCount == 4)
    {
        float left = vertices[3].x;
        float bottom = vertices[3].y;
        float right = vertices[1].x;
        float top = vertices[1].y;
        _boundsRektangleBatcher.addRektangle(left, bottom, right, top);
    }
    else if (vertexCount == 3)
    {
        float leftX = vertices[2].x;
        float leftY = vertices[2].y;
        float topX = vertices[1].x;
        float topY = vertices[1].y;
        float rightX = vertices[0].x;
        float rightY = vertices[0].y;
        _boundsTriangleBatcher.addTriangle(leftX, leftY, topX, topY, rightX, rightY);
    }
    _boundsRektangleBatcher.end(*_shader, *_matrix, c);
    _boundsTriangleBatcher.end(*_shader, *_matrix, c);
}

void Box2DDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    {
        Color c = Color(color.r, color.g, color.b, 0.3f);
        _fillRektangleBatcher.begin();
        _fillTriangleBatcher.begin();
        if (vertexCount == 4)
        {
            float left = vertices[3].x;
            float bottom = vertices[3].y;
            float right = vertices[1].x;
            float top = vertices[1].y;
            _fillRektangleBatcher.addRektangle(left, bottom, right, top);
        }
        else if (vertexCount == 3)
        {
            float leftX = vertices[2].x;
            float leftY = vertices[2].y;
            float topX = vertices[1].x;
            float topY = vertices[1].y;
            float rightX = vertices[0].x;
            float rightY = vertices[0].y;
            _fillTriangleBatcher.addTriangle(leftX, leftY, topX, topY, rightX, rightY);
        }
        _fillRektangleBatcher.end(*_shader, *_matrix, c);
        _fillTriangleBatcher.end(*_shader, *_matrix, c);
    }

    {
        Color c = Color(1, 0, 0, 0.3f);
        _boundsRektangleBatcher.begin();
        _boundsTriangleBatcher.begin();
        if (vertexCount == 4)
        {
            float left = vertices[3].x;
            float bottom = vertices[3].y;
            float right = vertices[1].x;
            float top = vertices[1].y;
            _boundsRektangleBatcher.addRektangle(left, bottom, right, top);
        }
        else if (vertexCount == 3)
        {
            float leftX = vertices[2].x;
            float leftY = vertices[2].y;
            float topX = vertices[1].x;
            float topY = vertices[1].y;
            float rightX = vertices[0].x;
            float rightY = vertices[0].y;
            _boundsTriangleBatcher.addTriangle(leftX, leftY, topX, topY, rightX, rightY);
        }
        _boundsRektangleBatcher.end(*_shader, *_matrix, c);
        _boundsTriangleBatcher.end(*_shader, *_matrix, c);
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

void Box2DDebugRenderer::createDeviceDependentResources()
{
    _circleBatcher.createDeviceDependentResources();
    _fillRektangleBatcher.createDeviceDependentResources();
    _boundsRektangleBatcher.createDeviceDependentResources();
    _fillTriangleBatcher.createDeviceDependentResources();
    _boundsTriangleBatcher.createDeviceDependentResources();
}

void Box2DDebugRenderer::destroyDeviceDependentResources()
{
    _circleBatcher.destroyDeviceDependentResources();
    _fillRektangleBatcher.destroyDeviceDependentResources();
    _boundsRektangleBatcher.destroyDeviceDependentResources();
    _fillTriangleBatcher.destroyDeviceDependentResources();
    _boundsTriangleBatcher.destroyDeviceDependentResources();
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
