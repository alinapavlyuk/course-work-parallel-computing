#include "Invertedndex.h"

std::vector<std::string> InvertedIndex::tokenizeAlphanumeric(const std::string& content) {
    std::vector<std::string> tokens;
    std::string token;

    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];

        if (std::isalnum(c)) {
            token += std::tolower(c);
        } else if (c == '\'' && (i + 1 < content.size()) &&
                   (content[i + 1] == 's' || content[i + 1] == 't')) {
            token += c;
        } else if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

void InvertedIndex::build() {
    std::string path = "../data";

    for (const auto & entry : fs::directory_iterator(path)) {
        std::string file_contents, line;
        std::ifstream file(entry.path());

        while (getline (file, line)) {
            file_contents += line;
            file_contents.push_back(' ');
        }

        std::cout << entry.path() << std::endl;

        this->add_document(file_contents);
    }
}

void InvertedIndex::add_document(const std::string &content) {
    this->lastID++;

    std::vector<std::string> tokens = tokenizeAlphanumeric(content);
    for (const auto& token : tokens) {
        this->index[token].insert(this->lastID);
    }

    this->display();
}

void InvertedIndex::display() const {
    for (const auto& [word, docIDs] : this->index) {
        std::cout << word  << ": ";

        for (auto it=docIDs.begin(); it != docIDs.end(); ++it) {
            std::cout << *it << " ";
        }

        std::cout << "\n";
    }
}

std::vector<int> InvertedIndex::search_by_keys(std::vector<std::string> keys) {}