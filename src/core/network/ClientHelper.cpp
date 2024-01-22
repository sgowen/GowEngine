//
//  ClientHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ClientHelper::ClientHelper(PacketHandler* packetHandler) : NetworkHelper(packetHandler), _state(CLIENT_NOT_READY_TO_SAY_HELLO)
{
    // Empty
}

ClientHelper::~ClientHelper()
{
    // Empty
}

int ClientHelper::getState()
{
    return _state;
}
