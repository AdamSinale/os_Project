#include "Threadpool.hpp"

// ThreadPool constructor
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    // Create worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { workerFunction(); });
    }
}

// ThreadPool destructor
ThreadPool::~ThreadPool() {
    {
        // Lock the queue and set the stop flag to true
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    // Notify all worker threads to wake up
    condition.notify_all();

    // Join all threads to ensure proper cleanup
    for (std::thread &worker : workers) {
        worker.join();
    }
}

// Method to enqueue a new task into the thread pool
void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        // Lock the task queue
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }
    // Notify one worker thread that a new task is available
    condition.notify_one();
}

// Function executed by each worker thread
void ThreadPool::workerFunction() {
    while (true) {
        std::function<void()> task;
        {
            // Lock the queue and wait for a task or stop signal
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            // If stop flag is true and no tasks remain, exit the worker function
            if (stop && tasks.empty()) {
                return;
            }

            // Retrieve the next task from the queue
            task = std::move(tasks.front());
            tasks.pop();
        }

        // Execute the task
        task();
    }
}
