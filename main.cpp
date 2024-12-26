#include "src/Server.h"
#include "src/FileManager.h"

#define EXECUTORS_NUM 6

int main() {
    ThreadPool thread_pool;
    thread_pool.initialize(EXECUTORS_NUM);

    FileManager file_manager("../data", &thread_pool);
    file_manager.build_index();

    Server server(&thread_pool, &file_manager, 8002);
    server.start();

    return 0;
}
