//
//  Box2DDebugRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/22/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <box2d/b2_draw.h>

#include "PolygonBatcher.hpp"
#include "CircleBatcher.hpp"

struct Shader;
class b2World;

class Box2DDebugRenderer : public b2Draw
{
public:
    Box2DDebugRenderer(int maxBatchSize);
    virtual ~Box2DDebugRenderer();
    
    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform(const b2Transform& xf);

    /// Draw a point.
    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color);
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void render(Shader* shader, mat4* matrix, b2World* world);
    
private:
    PolygonBatcher _fillPolygonBatcher;
    PolygonBatcher _boundsPolygonBatcher;
    CircleBatcher _circleBatcher;
    Shader* _shader;
    mat4* _matrix;
};
