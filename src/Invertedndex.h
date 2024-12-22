#ifndef COURSE_WORK_INVERTEDNDEX_H
#define COURSE_WORK_INVERTEDNDEX_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>

namespace fs = std::filesystem;

class InvertedIndex {
private:
    std::unordered_map<std::string, std::set<int>> index;
    unsigned int lastID = 0;

    static std::vector<std::string> tokenizeAlphanumeric(const std::string& content);
    void display() const;

public:
    void build();
    void add_document(const std::string& content);
    std::vector<int> search_by_keys(const std::vector<std::string>& keys);
};

#endif //COURSE_WORK_INVERTEDNDEX_H
