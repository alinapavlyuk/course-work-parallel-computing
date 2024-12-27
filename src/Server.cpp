#include "Server.h"

int Server::start() {
    if (this->thread_pool->working()) {
        int sockfd = create_and_bind_socket();
        if (sockfd == 0) {
            return 1;
        }

        int host_addr_len = sizeof(this->host_addr);

        if (listen(sockfd, SOMAXCONN) != 0) {
            perror("Failed to listen on socket");
            return 1;
        }
        printf("Server listening for connections\n");

        for (;;) {
            int newsockfd = accept(sockfd, (sockaddr *)&host_addr,
                                   (socklen_t *)&host_addr_len);
            if (newsockfd < 0) {
                perror("Failed to accept connection");
                continue;
            }
            printf("Conection accepted\n");

            Task new_task = Task([newsockfd, this]{
                process_request(newsockfd);
            });
            this->thread_pool->add_task(new_task);
        }
    } else {
        printf("Thread pool should work by the time server's work starts.");
    }
}

int Server::create_and_bind_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create a socket");
        return 0;
    }
    printf("Socket created successfuly\n");

    int host_addr_len = sizeof(this->host_addr);

    this->host_addr.sin_family = AF_INET;
    this->host_addr.sin_port = htons(port);
    this->host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&this->host_addr, host_addr_len) != 0) {
        perror("Failed to bind a socket");
        return 0;
    }
    printf("Socket bound to address successfuly\n");

    return sockfd;
}

void Server::process_request(int sockfd) {
    char buffer[BUFFER_SIZE];

    int valread = read(sockfd, buffer, BUFFER_SIZE);
    if (valread < 0) {
        perror("Failed to read from socket");
        return;
    }

    char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
    sscanf(buffer, "%s %s %s", method, uri, version);
    printf("%s %s %s\n", method, version, uri);

    std::string path = get_request_path(uri);
    auto [param_name, param_value] = get_request_params(uri);

    printf("Path: %s\n", path.c_str());
    printf("Param %s value: %s", param_name.c_str(), param_value.c_str());
    printf("\n");

    std::string status_line = "HTTP/1.0 200 OK\r\n";
    std::string headers = "Server: webserver-c\r\nContent-type: text/html\r\n\r\n";
    std::string contents;

    if (path == "/search") {
        if (param_value.empty() || param_name != "q") {
            status_line = "HTTP/1.0 400 Bad Request\r\n";
            contents = "Error: Parameter (q) must be provided for search.";
        } else {
            std::vector<int> result = this->file_manager->search_documents_by_keywords(param_value);

            printf("Returning search result\n");
            contents = "Search result: ";
            for (const auto& fileID : result) {
                contents += " " + std::to_string(fileID);
            }
        }
    } else if (path == "/download") {
        if (param_value.empty() || param_name != "id") {
            status_line = "HTTP/1.0 400 Bad Request\r\n";
            contents = "Error: Parameter (id) must be provided for download.";
        } else {
            std::string file_name = this->file_manager->translate_fileID_to_filename(std::stoi(param_value));

            if (file_name.empty()) {
                status_line = "HTTP/1.0 400 Bad Request\r\n";
                contents = "Error: No file with such ID exists.";
            } else {
                std::string result = FileManager::get_file_contents_by_name(file_name);

                printf("Returning document\n");
                contents = "File " + param_value + " contents:\n";
                contents += result;
            }
        }
    } else {
        printf("Returning 404\n");
        status_line = "HTTP/1.0 404 NOT FOUND\r\n";
        contents = "Error: Not Found";
    }

    std::string response = status_line + headers + contents + "\r\n";

    int valwrite = write(sockfd, response.c_str(), response.size());
    if (valwrite < 0) {
        perror("Failed to write to socket");
        return;
    }

    close(sockfd);
}

std::string Server::get_request_path(const char* uri) {
    std::string uri_str(uri);
    size_t question_mark_pos = uri_str.find('?');
    if (question_mark_pos != std::string::npos) {
        return uri_str.substr(0, question_mark_pos);
    }
    return uri_str;
}

std::pair<std::string, std::string> Server::get_request_params(const char* uri) {
    std::string param_name;
    std::string param_value;

    std::string uri_str(uri);

    size_t question_mark_pos = uri_str.find('?');
    size_t equal_sign_pos = uri_str.find('=');

    param_name = uri_str.substr(question_mark_pos + 1, equal_sign_pos - question_mark_pos - 1);
    param_value = uri_str.substr(equal_sign_pos + 1);

    std::string replace_sign = "+";
    std::string replace_by = " ";

    size_t pos = param_value.find(replace_sign);
    while (pos != std::string::npos) {
        param_value.replace(pos, replace_sign.size(), replace_by);

        pos = param_value.find(replace_sign,
                         pos + replace_by.size());
    }

    return {param_name, param_value};
}




