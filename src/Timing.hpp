//
//  Timing.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class Timing
{
public:
    Timing();
    ~Timing() {}
    
    void onFrame();
    void reset();
    float getTime() const;
    
private:
    double _time;
};
