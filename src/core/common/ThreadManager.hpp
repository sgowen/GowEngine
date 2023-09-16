//
//  ThreadManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <string>
#include <thread>

class ThreadManager
{
public:
    static ThreadManager& getInstance()
    {
        static ThreadManager ret = ThreadManager();
        return ret;
    }
    
    void spawnThread(std::string threadID, void (*func)(void*), void* arg);
    void tearDownThreadIfRunning(std::string threadID);
    void tearDownThread(std::string threadID);
    bool isThreadRunning(std::string threadID);
    
private:
    std::map<std::string, std::thread*> _threads;
    
    ThreadManager() {}
    ~ThreadManager() {}
    ThreadManager(const ThreadManager&);
    ThreadManager& operator=(const ThreadManager&);
};
