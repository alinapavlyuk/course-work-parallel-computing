#ifndef COURSE_WORK_INVERTEDNDEX_H
#define COURSE_WORK_INVERTEDNDEX_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <set>
#include <shared_mutex>
#include "SegmentedHashMap.h"

namespace fs = std::filesystem;


class InvertedIndex {
private:
    SegmentedHashMap<std::string, std::set<int>> index;
//    std::unordered_map<std::string, std::set<int>> index;
    std::atomic<int> lastID = 0;

    mutable read_write_lock index_rw_lock;
    static std::vector<std::string> tokenize_alphanumeric(const std::string& content);

public:
    InvertedIndex(size_t segment_num): index{segment_num} {};

    void display() const;
    int add_document(const std::string& content);
    std::vector<int> search_by_keys(const std::string& keys);
};

#endif //COURSE_WORK_INVERTEDNDEX_H
