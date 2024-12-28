#ifndef COURSE_WORK_INVERTEDNDEX_H
#define COURSE_WORK_INVERTEDNDEX_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <set>
#include <shared_mutex>

namespace fs = std::filesystem;

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;

class InvertedIndex {
private:
    std::unordered_map<std::string, std::set<int>> index;
    std::atomic<int> lastID = 0;

    mutable read_write_lock index_rw_lock;
    static std::vector<std::string> tokenize_alphanumeric(const std::string& content);

public:
    void display() const;
    int add_document(const std::string& content);
    std::vector<int> search_by_keys(const std::string& keys);
};

#endif //COURSE_WORK_INVERTEDNDEX_H
