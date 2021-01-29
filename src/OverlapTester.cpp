//
//  OverlapTester.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "OverlapTester.hpp"

#include "Vector2.hpp"
#include "Circle.hpp"
#include "Rektangle.hpp"
#include "Triangle.hpp"
#include "Line.hpp"

#include "MathUtil.hpp"

#include <stdio.h>
#include <stdlib.h>

bool OverlapTester::doCirclesOverlap(Circle& c1, Circle& c2)
{
    Vector2 c1Center = c1._center;
    Vector2 c2Center = c2._center;
    float distance = c1Center.distSquared(c2Center);
    float radiusSum = c1._radius + c2._radius;
    
    return distance <= radiusSum * radiusSum;
}

bool OverlapTester::doRektanglesOverlap(Rektangle& r1, Rektangle& r2)
{
    if (r1._angle > 0)
    {
        float halfWidth = r1._width / 2;
        float halfHeight = r1._height / 2;
        
        float rad = DEGREES_TO_RADIANS(r1._angle);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        float x = r1.left() + r1._width / 2;
        float y = r1.bottom() + r1._height / 2;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        return isPointInRektangle(Vector2(x1, y1), r2) || isPointInRektangle(Vector2(x2, y2), r2) || isPointInRektangle(Vector2(x3, y3), r2) || isPointInRektangle(Vector2(x4, y4), r2);
    }
    else
    {
        return (r1.left() < r2.left() + r2._width && r1.left() + r1._width > r2.left() && r1.bottom() < r2.top() && r1.top() > r2.bottom());
    }
}

bool OverlapTester::overlapCircleRektangle(Circle& c, Rektangle& r)
{
    float closestX = c._center.x();
    float closestY = c._center.y();
    
    if (c._center.x() < r.left())
    {
        closestX = r.left();
    }
    else if (c._center.x() > r.left() + r._width)
    {
        closestX = r.left() + r._width;
    }
    
    if (c._center.y() < r.bottom())
    {
        closestY = r.bottom();
    }
    else if (c._center.y() > r.top())
    {
        closestY = r.top();
    }
    
    return c._center.distSquared(closestX, closestY) < c._radius * c._radius;
}

bool OverlapTester::doesRektangleOverlapTriangle(Rektangle& r, Triangle& t)
{
    return doLineAndRektangleOverlap(t._sideA, r) || doLineAndRektangleOverlap(t._sideB, r) || doLineAndRektangleOverlap(t._sideC, r);
}

bool OverlapTester::isPointInRektangle(Vector2 p, Rektangle& r)
{
    return isPointInRektangle(p.x(), p.y(), r);
}

bool OverlapTester::isPointInRektangle(float x, float y, Rektangle& r)
{
    return r.left() <= x && r.left() + r._width >= x && r.bottom() <= y && r.top() >= y;
}

bool OverlapTester::isPointInCircle(Vector2& p, Circle& c)
{
    return c._center.distSquared(p) < c._radius * c._radius;
}

bool OverlapTester::isPointInTriangle(Vector2& p, Triangle& tr)
{
    float pX = p.x();
    float pY = p.y();
    float p0X = tr.getPointA().x();
    float p0Y = tr.getPointA().y();
    float p1X = tr.getPointB().x();
    float p1Y = tr.getPointB().y();
    float p2X = tr.getPointC().x();
    float p2Y = tr.getPointC().y();
    
    /* Calculate area of triangle ABC */
    float a = calcAreaOfTriangle(p0X, p0Y, p1X, p1Y, p2X, p2Y);
    
    /* Calculate area of triangle PBC */
    float a1 = calcAreaOfTriangle (pX, pY, p1X, p1Y, p2X, p2Y);
    
    /* Calculate area of triangle PAC */
    float a2 = calcAreaOfTriangle (p0X, p0Y, pX, pY, p2X, p2Y);
    
    /* Calculate area of triangle PAB */
    float a3 = calcAreaOfTriangle (p0X, p0Y, p1X, p1Y, pX, pY);
    
    float aSum = a1 + a2 + a3;
    
    /* Check if sum of A1, A2 and A3 is same as A */
    return a < (aSum + 0.1f) && a > (aSum - 0.1f);
}

bool OverlapTester::doLineAndRektangleOverlap(Line& l, Rektangle& r)
{
    if (doLinesIntersect(l._origin.x(), l._origin.y(), l._end.x(), l._end.y(), r.left(), r.bottom() + r._height / 2, r.left() + r._width, r.bottom() + r._height / 2))
    {
        return true;
    }
    
    if (doLinesIntersect(l._origin.x(), l._origin.y(), l._end.x(), l._end.y(), r.left() + r._width / 2, r.bottom(), r.left() + r._width / 2, r.top()))
    {
        return true;
    }
    
    return false;
}

float OverlapTester::calcAreaOfTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float sideADistance = sqrtf(powf((x2 - x1), 2) + powf((y2 - y1), 2));
    float sideBDistance = sqrtf(powf((x3 - x2), 2) + powf((y3 - y2), 2));
    float sideCDistance = sqrtf(powf((x1 - x3), 2) + powf((y1 - y3), 2));
    
    float sp = (sideADistance + sideBDistance + sideCDistance) / 2;
    
    return sqrtf(sp * (sp - sideADistance) * (sp - sideBDistance) * (sp - sideCDistance));
}

bool OverlapTester::doLinesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    // Return false if either of the lines have zero length
    if ((x1 == x2 && y1 == y2) || (x3 == x4 && y3 == y4))
    {
        return false;
    }
    // Fastest method, based on Franklin Antonio's "Faster Line Segment Intersection" topic "in Graphics Gems III" book (http://www.graphicsgems.org/)
    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x3 - x4;
    double by = y3 - y4;
    double cx = x1 - x3;
    double cy = y1 - y3;
    
    double alphaNumerator = by * cx - bx * cy;
    double commonDenominator = ay * bx - ax * by;
    if (commonDenominator > 0)
    {
        if (alphaNumerator < 0 || alphaNumerator > commonDenominator)
        {
            return false;
        }
    }
    else if (commonDenominator < 0)
    {
        if (alphaNumerator > 0 || alphaNumerator < commonDenominator)
        {
            return false;
        }
    }
    double betaNumerator = ax * cy - ay * cx;
    if (commonDenominator > 0)
    {
        if (betaNumerator < 0 || betaNumerator > commonDenominator)
        {
            return false;
        }
    }
    else if (commonDenominator < 0)
    {
        if (betaNumerator > 0 || betaNumerator < commonDenominator)
        {
            return false;
        }
    }
    if (commonDenominator == 0)
    {
        // This code wasn't in Franklin Antonio's method. It was added by Keith Woodward.
        // The lines are parallel.
        // Check if they're collinear.
        double y3LessY1 = y3 - y1;
        double collinearityTestForP3 = x1 * (y2 - y3) + x2 * (y3LessY1) + x3 * (y1 - y2); // see http://mathworld.wolfram.com/Collinear.html
        // If p3 is collinear with p1 and p2 then p4 will also be collinear, since p1-p2 is parallel with p3-p4
        if (collinearityTestForP3 == 0)
        {
            // The lines are collinear. Now check if they overlap.
            if ((x1 >= x3 && x1 <= x4) || (x1 <= x3 && x1 >= x4) || (x2 >= x3 && x2 <= x4) || (x2 <= x3 && x2 >= x4) || (x3 >= x1 && x3 <= x2) || (x3 <= x1 && x3 >= x2))
            {
                if ((y1 >= y3 && y1 <= y4) || (y1 <= y3 && y1 >= y4) || (y2 >= y3 && y2 <= y4) || (y2 <= y3 && y2 >= y4) || (y3 >= y1 && y3 <= y2) || (y3 <= y1 && y3 >= y2))
                {
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}
