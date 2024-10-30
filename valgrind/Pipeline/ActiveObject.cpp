#include "ActiveObject.hpp"

// Constructor: Initializes the worker thread
ActiveObject::ActiveObject() : running(true) {
    // Start the worker thread
    worker = std::thread(&ActiveObject::run, this);
}

// Destructor: Ensures the thread is stopped and joined
ActiveObject::~ActiveObject() {
    stop();
}

// submitTask: Adds a task to the queue and notifies the worker thread
void ActiveObject::submitTask(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
    cv.notify_one(); // Wake up the worker thread
}

// stop: Signals the thread to stop and joins it
void ActiveObject::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running = false;
    }
    cv.notify_one(); // Unblock the worker thread if it's waiting
    if (worker.joinable()) {
        worker.join(); // Wait for the thread to finish
    }
}

// run: The main loop for the worker thread, processing tasks from the queue
void ActiveObject::run() {
    while (true) {
        std::function<void()> task;
        {
            // Lock the queue and wait for tasks or stop signal
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this] { return !taskQueue.empty() || !running; });

            if (!running && taskQueue.empty()) {
                break; // Exit the loop if stopped and no tasks remain
            }

            // Get the next task from the queue
            task = taskQueue.front();
            taskQueue.pop();
        }
        // Execute the task outside the lock to avoid blocking other submissions
        task();
    }
}
