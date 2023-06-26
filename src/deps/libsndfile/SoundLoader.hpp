//
//  SoundLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/30/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct Sound;

class SoundLoader
{
public:
    void loadData(Sound& s);
    void freeData(Sound& s);
};
