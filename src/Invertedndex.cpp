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

int InvertedIndex::add_document(const std::string& content) {
    int currentID = this->lastID.fetch_add(1);

    std::vector<std::string> tokens = tokenize_alphanumeric(content);

    for (const auto& token : tokens) {
        index.insert_to_set(token, currentID);
    }

    return currentID;
}

void InvertedIndex::display() const {
    index.apply_to_all([](const std::string& key, const std::set<int>& fileIDs) {
        printf("%s: ", key.c_str());
        for (const auto& id : fileIDs) {
            printf("%d ", id);
        }
        printf("\n");
    });
}

std::vector<int> InvertedIndex::search_by_keys(const std::string& keys) {
    std::set<int> result;

    printf("Keys: %s\n", keys.c_str());

    std::vector<std::string> tokenized_keys = tokenize_alphanumeric(keys);

    if (tokenized_keys.empty()) {
        return {};
    }

    std::set<int> temp_result;
    bool first_key = true;

    for (const auto& key : tokenized_keys) {
        std::set<int> current_set;
        if (index.find(key, current_set)) {
            if (first_key) {
                result = std::move(current_set);
                first_key = false;
            } else {
                temp_result.clear();
                std::set_intersection(result.begin(), result.end(),
                                      current_set.begin(), current_set.end(),
                                      std::inserter(temp_result, temp_result.begin()));
                result = std::move(temp_result);
            }
        } else {
            return {};
        }
    }

    return {result.begin(), result.end()};
}
