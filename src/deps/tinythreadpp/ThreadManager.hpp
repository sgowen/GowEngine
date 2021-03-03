//
//  ThreadManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <tinythread.h>

#include <map>
#include <string>

#define THREAD_MGR ThreadManager::getInstance()

class ThreadManager
{
public:
    static ThreadManager& getInstance()
    {
        static ThreadManager ret = ThreadManager();
        return ret;
    }
    
    void spawnThread(std::string threadID, void (*func)(void *), void* arg);
    void tearDownThread(std::string threadID);
    bool isThreadRunning(std::string threadID);
    
private:
    std::map<std::string, tthread::thread*> _threads;
    
    ThreadManager() {}
    ~ThreadManager() {}
    ThreadManager(const ThreadManager&);
    ThreadManager& operator=(const ThreadManager&);
};
