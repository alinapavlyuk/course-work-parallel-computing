#include "src/Server.h"
#include "src/Invertedndex.h"

#define EXECUTORS_NUM 6

int main() {
    ThreadPool thread_pool;
    thread_pool.initialize(EXECUTORS_NUM);

    InvertedIndex index;
    index.build();

    Server server(&thread_pool, 8082);
    server.start();

    return 0;
}
