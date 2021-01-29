//
//  CursorConverter.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

struct CursorEvent;

#define CURSOR_CONVERTER (CursorConverter::getInstance())

class CursorConverter
{
public:
    static CursorConverter& getInstance();
    
    Vector2& convert(CursorEvent& ce);
    Vector2& convert(Vector2& v);
    void setCursorSize(int cursorWidth, int cursorHeight);
    void setMatrixSize(float matrixWidth, float matrixHeight);
    
private:
    Vector2 _cursor;
    int _cursorWidth;
    int _cursorHeight;
    float _matrixWidth;
    float _matrixHeight;
    
    CursorConverter();
    ~CursorConverter();
    CursorConverter(const CursorConverter&);
    CursorConverter& operator=(const CursorConverter&);
};
