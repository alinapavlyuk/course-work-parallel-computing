#include "ThreadPool.h"

void ThreadPool::initialize(const size_t worker_count) {
    write_lock _(tp_rw_lock);

    if (initialized || terminated) {
        return;
    }
    workers.reserve(worker_count);

    for (size_t id = 1; id <= worker_count; id++) {
        workers.emplace_back(&ThreadPool::routine, this);
    }
    initialized = !workers.empty();
}

void ThreadPool::terminate()
{
    {
        write_lock _(tp_rw_lock);

        if (working_unsafe()) {
            terminated = true;
        } else {
            std::printf("Already terminated.\n");
            workers.clear();
            terminated = false;
            initialized = false;
            return;
        }
    }

    task_waiter.notify_all();

    for (std::thread &worker: workers) {
        worker.join();
    }

    workers.clear();
    terminated = false;
    initialized = false;
}

void ThreadPool::urgent_terminate() {
    {
        write_lock _(tp_rw_lock);

        if (working_unsafe()) {
            terminated = true;
            tasks.clear();
        } else {
            workers.clear();
            terminated = false;
            initialized = false;
            return;
        }
    }

    task_waiter.notify_all();

    for (std::thread &worker: workers) {
        worker.join();
    }

    workers.clear();
    terminated = false;
    initialized = false;
}



void ThreadPool::routine()
{
    while (true) {
        bool task_acquired = false;

        {
            write_lock _(tp_rw_lock);
            auto wait_condition = [this] {
                return !paused;
            };
            unpause_waiter.wait(_, wait_condition);
        }

        Task task;
        {
            write_lock _(tp_rw_lock);
            auto wait_condition = [this, &task_acquired, &task] {
                task_acquired = tasks.pop(task);
                return terminated || task_acquired;
            };

            task_waiter.wait(_, wait_condition);
        }

        if (terminated && !task_acquired) {
            return;
        }

        task.function();
    }
}

void ThreadPool::pause() {
    write_lock _(tp_rw_lock);
    paused = true;
}

void ThreadPool::unpause() {
    write_lock _(tp_rw_lock);
    paused = false;
    unpause_waiter.notify_all();
}

bool ThreadPool::working() const {
    read_lock _(tp_rw_lock);
    return working_unsafe();
}

bool ThreadPool::working_unsafe() const {
    return initialized && !terminated;
}

void ThreadPool::add_task(Task& task) {
    if (!working()) {
        return;
    }

    tasks.emplace(task);
    task_waiter.notify_one();
}
