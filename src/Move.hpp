//
//  Move.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class InputState;
class OutputMemoryBitStream;
class InputMemoryBitStream;

class Move
{
public:
    Move(InputState* inputState, float timestamp);
    Move(InputState* inputState);
    ~Move();
    
    void write(OutputMemoryBitStream& outputStream) const;
    void read(InputMemoryBitStream& inputStream);
    bool isEqual(const Move* move) const;
    InputState* getInputState() const;
    float getTimestamp() const;
    void setTimestamp(float timeStamp);
    void copyInputState(InputState* inputState);
    
private:
    InputState* _inputState;
    float _timestamp;
};
