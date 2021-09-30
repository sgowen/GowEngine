//
//  ThreadManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ThreadManager::spawnThread(std::string threadID, void (*func)(void *), void* arg)
{
    assert(_threads.find(threadID) == _threads.end());
    
    _threads.emplace(threadID, new std::thread {func, arg});
}

void ThreadManager::tearDownThreadIfRunning(std::string threadID)
{
    if (!isThreadRunning(threadID))
    {
        return;
    }
    
    tearDownThread(threadID);
}

void ThreadManager::tearDownThread(std::string threadID)
{
    auto q = _threads.find(threadID);
    assert(q != _threads.end());
    q->second->join();
    delete q->second;
    _threads.erase(q);
}

bool ThreadManager::isThreadRunning(std::string threadID)
{
    auto q = _threads.find(threadID);
    return q != _threads.end();
}
