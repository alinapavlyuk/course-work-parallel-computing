#ifndef COURSE_WORK_TASKQUEUE_H
#define COURSE_WORK_TASKQUEUE_H

#include <queue>
#include <thread>
#include <shared_mutex>
#include <iostream>
#include <utility>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;

struct Task {
    std::function<void()> function;

    Task() : function(std::function<void()>()) {}
    Task(std::function<void()> task_function) :
            function(std::move(task_function))
    {}
};

class TaskQueue {
    using task_queue_implementation = std::vector<Task>;

public:
    inline TaskQueue() {};
    inline ~TaskQueue() { clear(); }
    inline bool empty() const;
    size_t get_size() const;

public:
    void clear();
    bool pop(Task& task);
    bool emplace(Task& task);

public:
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue(TaskQueue&& other) = delete;
    TaskQueue& operator=(const TaskQueue& rhs) = delete;
    TaskQueue& operator=(TaskQueue&& rhs) = delete;

private:
    mutable read_write_lock tq_rw_lock; //task queue lock
    task_queue_implementation tasks;
};

#endif //COURSE_WORK_TASKQUEUE_H
