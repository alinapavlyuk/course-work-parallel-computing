#ifndef COURSE_WORK_SERVER_H
#define COURSE_WORK_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstring>
#include "ThreadPool.h"

#define BUFFER_SIZE 2048

class Server {
    ThreadPool* thread_pool;
    sockaddr_in host_addr;
    int port;

public:
    Server(ThreadPool* thread_pool, int port) {
        this->thread_pool = thread_pool;
        this->port = port;
    }
    int start();

private:
    int create_and_bind_socket();
    static void process_request(int sockfd);
    static std::vector<char*> get_request_params(char *uri);
};

#endif //COURSE_WORK_SERVER_H
