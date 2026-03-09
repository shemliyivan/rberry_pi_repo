#include "ThreadSafeQueue.hpp"
#include <string>
#include <mutex>
#include <condition_variable>

void ThreadSafeQueue::push(std::string &received_bytes)
{
    std::lock_guard<std::mutex> lock(mutexTSQ);
    queueTSQ.push(received_bytes);
    condTSQ.notify_one();    
}

std::string ThreadSafeQueue::pop()
{
    std::unique_lock<std::mutex> lock(mutexTSQ);
    condTSQ.wait(lock, [this](){return !this->queueTSQ.empty();});
    std::string received_bytes = queueTSQ.front();
    queueTSQ.pop();
    return received_bytes;
}