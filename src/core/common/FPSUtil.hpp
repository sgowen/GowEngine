//
//  FPSUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class FPSUtil
{
public:
    static FPSUtil& getInstance()
    {
        static FPSUtil ret = FPSUtil();
        return ret;
    }
    
    void update(double deltaTime);
    void onFrame();
    int fps() const;
    
private:
    double _stateTime;
    int _fPS;
    int _numFrames;
    
    FPSUtil();
    ~FPSUtil() {}
    FPSUtil(const FPSUtil&);
    FPSUtil& operator=(const FPSUtil&);
};
