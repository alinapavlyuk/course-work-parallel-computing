#include "Invertedndex.h"

std::vector<std::string> InvertedIndex::tokenize_alphanumeric(const std::string& content) {
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

int InvertedIndex::add_document(const std::string &content) {
    this->lastID.fetch_add(1);
    int currentID = this->lastID.load();

    std::vector<std::string> tokens = tokenize_alphanumeric(content);

    {
        write_lock _(index_rw_lock);
        for (const auto& token : tokens) {
            this->index[token].insert(currentID);
        }
    }

    return currentID;
}

void InvertedIndex::display() const {
    read_lock _(index_rw_lock);
    for (const auto& [word, fileIDs] : this->index) {
        printf("%s: ", word.c_str());

        for (auto it=fileIDs.begin(); it != fileIDs.end(); ++it) {
            printf("%d ", *it);
        }

        printf("\n");
    }
}

std::vector<int> InvertedIndex::search_by_keys(const std::string& keys) {
    std::set<int> result;

    printf("Keys: %s\n", keys.c_str());

    std::vector<std::string> tokenized_keys = tokenize_alphanumeric(keys);

    if (tokenized_keys.empty()) {
        return {};
    }

    {
        read_lock _(index_rw_lock);

        auto it = this->index.find(tokenized_keys[0]);
        if (it != this->index.end()) {
            result.insert(it->second.begin(), it->second.end());
        } else {
            return {};
        }

        for (size_t i = 1; i < tokenized_keys.size(); ++i) {
            std::set<int> temp_result;
            auto it = this->index.find(tokenized_keys[i]);
            if (it != this->index.end()) {
                std::set_intersection(result.begin(), result.end(), it->second.begin(), it->second.end(),
                                      std::inserter(temp_result, temp_result.begin()));
                result = std::move(temp_result);
            } else {
                return {};
            }
        }
    }

    std::vector<int> common_result{result.begin(), result.end()};

    return common_result;
}
