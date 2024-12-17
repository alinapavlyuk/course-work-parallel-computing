#include <iostream>
#include "src/Server.h"

#define EXECUTORS_NUM 6

int main() {
    ThreadPool thread_pool;
    thread_pool.initialize(EXECUTORS_NUM);

    Server server(&thread_pool, 8082);
    server.start();

    return 0;
}
