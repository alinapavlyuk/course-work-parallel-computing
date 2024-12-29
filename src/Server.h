#ifndef COURSE_WORK_SERVER_H
#define COURSE_WORK_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstring>
#include "ThreadPool.h"
#include "FileManager.h"

#define BUFFER_SIZE 2048

class Server {
    ThreadPool* thread_pool;
    FileManager* file_manager;
    sockaddr_in host_addr;
    int port;

public:
    Server(ThreadPool* thread_pool, FileManager* file_manager, int port) {
        this->thread_pool = thread_pool;
        this->file_manager = file_manager;
        this->port = port;
    }
    int start();

private:
    int create_and_bind_socket();
    void process_request(int sockfd);
    static std::string get_request_path(const char* uri);
    static std::pair<std::string, std::string> get_request_params(const char* uri);
    static std::string escape_json_string(const std::string& str);
};

#endif //COURSE_WORK_SERVER_H
