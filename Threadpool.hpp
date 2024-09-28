#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>


/*
    Thread pool design pattern class.
*/

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    // Method to enqueue a task into the thread pool
    void enqueueTask(std::function<void()> task);

private:
    // Vector to hold worker threads
    std::vector<std::thread> workers;

    // Queue to hold tasks for the thread pool
    std::queue<std::function<void()>> tasks;

    // Mutex to synchronize access to the task queue
    std::mutex queueMutex;

    // Condition variable to notify worker threads of new tasks
    std::condition_variable condition;

    // Flag to indicate when the thread pool is stopping
    bool stop;

    // Function executed by each worker thread
    void workerFunction();
};
