//
//  ClipboardHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/02/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class ClipboardHandler
{
public:
    virtual const char* getClipboardString() = 0;
    
    ClipboardHandler() {}
    virtual ~ClipboardHandler() {}
};

class NullClipboardHandler : public ClipboardHandler
{
public:
    static NullClipboardHandler& getInstance()
    {
        static NullClipboardHandler ret = NullClipboardHandler();
        return ret;
    }
    
    virtual const char* getClipboardString()
    {
        return nullptr;
    }
    
private:
    NullClipboardHandler() {}
    virtual ~NullClipboardHandler() {}
};
