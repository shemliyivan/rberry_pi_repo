#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue{
    private:
        std::queue<std::string> queueTSQ;
        std::mutex mutexTSQ;
        std::condition_variable condTSQ;
    public:
        void push(std::string& received_bytes);
        std::string pop();
};

extern ThreadSafeQueue threadsafe_queue;