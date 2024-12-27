#include "FileManager.h"

void FileManager::build_index() {
    for (const auto & entry : fs::directory_iterator(this->data_path)) {
        std::string file_contents = this->get_file_contents_by_name(entry.path());

        std::cout << "Adding " << entry.path() << std::endl;

        Task new_task = Task([this, entry, &file_contents]{
            this->process_new_document(entry.path(), file_contents);
        });
        this->thread_pool->add_task(new_task);
    }
}

void FileManager::assign_file_watcher() {
    Task new_task = Task([this]{
        this->monitor_files();
    });
    this->thread_pool->add_task(new_task);
}

void FileManager::monitor_files() {
    while (true) {
        for (const auto & entry : fs::directory_iterator(this->data_path)) {
            if (!file_was_processed(entry.path())) {
                std::string file_contents = this->get_file_contents_by_name(entry.path());
                std::cout << "Adding " << entry.path() << std::endl;
                this->process_new_document(entry.path(), file_contents);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}

bool FileManager::file_was_processed(std::string file_name) {
    for (const auto& [fileID, name] : fileID_to_name_map) {
        if (name == file_name) {
            return true;
        }
    }
    return false;
}

void FileManager::process_new_document(const std::string file_name, const std::string &content) {
    int fileID = this->inverted_index.add_document(content);
    this->fileID_to_name_map[fileID] = file_name;
}

std::vector<int> FileManager::search_documents_by_keywords(const std::string& keywords) {
    std::vector<int> fileIDs = this->inverted_index.search_by_keys(keywords);
    return fileIDs;
}

std::string FileManager::translate_fileID_to_filename(int fileID) {
    auto it = fileID_to_name_map.find(fileID);
    std::string result = it == fileID_to_name_map.end() ? "" : it->second;
    return result;
}

std::string FileManager::get_file_contents_by_name(const std::string file_name) {
    std::string file_contents, line;
    std::ifstream file(file_name);

    while (getline (file, line)) {
        file_contents += line;
        file_contents.push_back(' ');
    }

    return file_contents;
}

