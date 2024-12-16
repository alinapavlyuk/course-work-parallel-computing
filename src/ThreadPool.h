#ifndef COURSE_WORK_THREADPOOL_H
#define COURSE_WORK_THREADPOOL_H

#include "TaskQueue.h"
#include <vector>
#include <functional>
#include <condition_variable>
#include <algorithm>

const size_t QUEUE_SIZE = 20;

class ThreadPool {
public:
    inline ThreadPool() = default;
    inline ~ThreadPool() { terminate(); }

public:
    void initialize(size_t worker_count);
    void terminate();
    void urgent_terminate();

    void routine();

    void pause();
    void unpause();

    bool working() const;
    bool working_unsafe() const;

public:
    void add_task(Task& task);

public:
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    ThreadPool& operator=(ThreadPool&& rhs) = delete;

private:
    mutable read_write_lock tp_rw_lock; //thread pool lock
    mutable std::condition_variable_any task_waiter;
    mutable std::condition_variable_any unpause_waiter;

    std::vector<std::thread> workers;
    TaskQueue tasks{QUEUE_SIZE};

    bool initialized = false;
    bool paused = false;
    bool terminated = false;
};

#endif //COURSE_WORK_THREADPOOL_H
