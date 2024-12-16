#include "TaskQueue.h"

bool TaskQueue::empty() const {
    read_lock _(tq_rw_lock);
    return tasks.empty();
}

size_t TaskQueue::get_size() const {
    read_lock _(tq_rw_lock);
    return tasks.size();
}

void TaskQueue::clear() {
    write_lock _(tq_rw_lock);
    while (!tasks.empty()) {
        tasks.erase(tasks.begin());
    }
}

bool TaskQueue::pop(Task& task) {
    write_lock _(tq_rw_lock);
    if (tasks.empty()) {
        return false;
    }
    else {
        task = std::move(tasks.front());
        tasks.erase(tasks.begin());
        return true;
    }
}

bool TaskQueue::emplace(Task& task) {
    write_lock _(tq_rw_lock);
    if(tasks.size() <= size) {
        tasks.emplace_back(task);
        return true;
    } else {
        return false;
    }
}
