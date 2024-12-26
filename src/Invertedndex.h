#ifndef COURSE_WORK_INVERTEDNDEX_H
#define COURSE_WORK_INVERTEDNDEX_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>

namespace fs = std::filesystem;

class InvertedIndex {
private:
    std::unordered_map<std::string, std::set<unsigned int>> index;
    unsigned int lastID = 0;

    static std::vector<std::string> tokenize_alphanumeric(const std::string& content);

public:
    void display() const;
    int add_document(const std::string& content);
    std::vector<int> search_by_keys(const std::string& keys);
};

#endif //COURSE_WORK_INVERTEDNDEX_H
