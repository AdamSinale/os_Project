#ifndef ACTIVE_OBJECT_HPP
#define ACTIVE_OBJECT_HPP

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class ActiveObject {
public:
    // Constructor and Destructor
    ActiveObject();
    ~ActiveObject();

    // Submit a new task to the active object
    void submitTask(std::function<void()> task);

    // Stop the active object
    void stop();

private:
    // Main loop for the worker thread
    void run();

    // Task queue and synchronization tools
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::thread worker;
    bool running;
};

#endif // ACTIVE_OBJECT_HPP
