//
//  OverlapTester.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct Vector2;
struct Circle;
struct Rektangle;
struct Triangle;
struct Line;

class OverlapTester
{
public:
    static bool doCirclesOverlap(Circle& c1, Circle& c2);
    static bool doRektanglesOverlap(Rektangle& r1, Rektangle& r2);
    static bool doesRektangleOverlapTriangle(Rektangle& r, Triangle& t);
    static bool overlapCircleRektangle(Circle& c, Rektangle& r);
    static bool isPointInRektangle(Vector2 p, Rektangle& r);
    static bool isPointInRektangle(float x, float y, Rektangle& r);
    static bool isPointInCircle(Vector2& p, Circle& c);
    static bool isPointInTriangle(Vector2& p, Triangle& t);
    static bool doLineAndRektangleOverlap(Line& l, Rektangle& r);

private:
    static float calcAreaOfTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    static bool doLinesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    
    OverlapTester();
    ~OverlapTester();
    OverlapTester(const OverlapTester&);
    OverlapTester& operator=(const OverlapTester&);
};
