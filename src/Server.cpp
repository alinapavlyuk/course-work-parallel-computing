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

            Task new_task = Task([newsockfd]{
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

    std::vector<char*> params = get_request_params(uri);

    printf("Path: %s\n", uri);
    printf("Params:");

    for (char* p : params) {
        printf(" %s", p);
    }
    printf("\n");

    const char* status_line = "HTTP/1.0 200 OK\r\n";
    const char* headers = "Server: webserver-c\r\n"
                          "Content-type: text/html\r\n\r\n";

    const char* contents;

    if (strcmp(uri, "/search") == 0) {
        printf("Returning search result\n");
        contents = "Search result";
    }
    else if (strcmp(uri, "/upload") == 0) {
        printf("Returning upload result\n");
        contents = "Upload result";
    }
    else {
        printf("Returning 404\n");
        status_line = "HTTP/1.0 404 NOT FOUND\r\n";
        contents = "Error";
    }

    char response[BUFFER_SIZE] = "";
    strcat(response, status_line);
    strcat(response, headers);
    strcat(response, contents);
    strcat(response, "\r\n");

    printf("RESPONSE\n%s\n", response);

    int valwrite = write(sockfd, response, strlen(response));
    if (valwrite < 0) {
        perror("Failed to write to socket");
        return;
    }

    close(sockfd);
}

std::vector<char *> Server::get_request_params(char *uri) {
    std::vector<char*> params;
    strtok(uri, "/?");
    char* param = strtok(NULL, "q=+");

    while (param != NULL)
    {
        params.push_back(param);
        param = strtok (NULL, "+");
    }

    return params;
}
