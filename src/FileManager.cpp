#include "FileManager.h"

void FileManager::build_index() {
    std::atomic<int> files_left_to_build = 0;

    for (const auto & entry : fs::directory_iterator(this->data_path)) {
        files_left_to_build.fetch_add(1);

//        printf("Adding %s\n", entry.path().c_str());

        Task new_task = Task([this, entry, &files_left_to_build]{
            std::string file_content = FileManager::get_file_content_by_name(entry.path());
            this->process_new_document(entry.path(), file_content);
            files_left_to_build.fetch_sub(1);
        });
        this->thread_pool->add_task(new_task);
    }

    while (files_left_to_build.load() > 0);
}

void FileManager::assign_file_watcher() {
    printf("File watcher starts\n");

    Task new_task = Task([this]{
        this->monitor_files();
    });
    this->thread_pool->add_task(new_task);
}

void FileManager::monitor_files() {
    while (true) {
        for (const auto & entry : fs::directory_iterator(this->data_path)) {
            if (!file_was_processed(entry.path())) {

                std::string file_content = FileManager::get_file_content_by_name(entry.path());
                printf("Adding %s\n", entry.path().c_str());
                this->process_new_document(entry.path(), file_content);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

bool FileManager::file_was_processed(const std::string& file_name) {
    read_lock _(map_rw_lock);
    for (const auto &[fileID, name]: fileID_to_name_map) {
        if (name == file_name) {
            return true;
        }
    }
    return false;
}

void FileManager::process_new_document(const std::string& file_name, const std::string &content) {
    int fileID = this->inverted_index.add_document(content);
    write_lock _(map_rw_lock);
    this->fileID_to_name_map[fileID] = file_name;
}

std::vector<int> FileManager::search_documents_by_keywords(const std::string& keywords) {
    std::vector<int> fileIDs = this->inverted_index.search_by_keys(keywords);
    return fileIDs;
}

std::string FileManager::translate_fileID_to_filename(int fileID) {
    read_lock _(map_rw_lock);
    auto it = fileID_to_name_map.find(fileID);
    std::string result = it == fileID_to_name_map.end() ? "" : it->second;
    return result;
}

std::string FileManager::get_file_content_by_name(const std::string file_name) {
    std::string file_content, line;
    std::ifstream file(file_name);

    while (getline (file, line)) {
        file_content += line;
        file_content.push_back(' ');
    }

    return file_content;
}

