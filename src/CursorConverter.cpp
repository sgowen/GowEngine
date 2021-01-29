//
//  CursorConverter.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "CursorConverter.hpp"

#include "CursorEvent.hpp"

CursorConverter& CursorConverter::getInstance()
{
    static CursorConverter ret = CursorConverter();
    return ret;
}

Vector2& CursorConverter::convert(CursorEvent& ce)
{
	_cursor.set((ce._x / _cursorWidth) * _matrixWidth, (1 - ce._y / _cursorHeight) * _matrixHeight);
    return _cursor;
}

Vector2& CursorConverter::convert(Vector2& v)
{
	_cursor.set((v.x() / _cursorWidth) * _matrixWidth, (1 - v.y() / _cursorHeight) * _matrixHeight);
    return _cursor;
}

void CursorConverter::setCursorSize(int cursorWidth, int cursorHeight)
{
    _cursorWidth = (float) cursorWidth;
    _cursorHeight = (float) cursorHeight;
}

void CursorConverter::setMatrixSize(float matrixWidth, float matrixHeight)
{
    _matrixWidth = matrixWidth;
    _matrixHeight = matrixHeight;
}

CursorConverter::CursorConverter() :
_cursor(),
_cursorWidth(1),
_cursorHeight(1),
_matrixWidth(1),
_matrixHeight(1)
{
    // Empty
}

CursorConverter::~CursorConverter()
{
    // Empty
}
