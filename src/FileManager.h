#ifndef COURSE_WORK_FILEMANAGER_H
#define COURSE_WORK_FILEMANAGER_H

#include "Invertedndex.h"
#include "ThreadPool.h"

class FileManager {
private:
    std::string data_path;
    InvertedIndex inverted_index;
    ThreadPool* thread_pool;
    std::unordered_map<int, std::string> fileID_to_name_map;

    mutable read_write_lock map_rw_lock;

    void monitor_files();
    bool file_was_processed(std::string file_name);

public:
    FileManager(std::string data_path, ThreadPool* thread_pool) {
        this->data_path = data_path;
        this->thread_pool = thread_pool;
    }

    void build_index();
    void assign_file_watcher();
    void process_new_document(std::string file_name, const std::string &content);
    std::vector<int> search_documents_by_keywords(const std::string& keywords);
    std::string translate_fileID_to_filename(int fileID);
    static std::string get_file_content_by_name(std::string file_name);
};

#endif //COURSE_WORK_FILEMANAGER_H
