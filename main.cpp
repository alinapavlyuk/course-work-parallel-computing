#include "src/Server.h"
#include "src/FileManager.h"

#define EXECUTORS_NUM 6

void test() {
    std::vector<int> threads_nums = {1, 2, 4, 6, 8, 16, 32};

    for (const auto threads_num: threads_nums) {
        ThreadPool thread_pool;
        thread_pool.initialize(EXECUTORS_NUM);

        FileManager file_manager("../data", &thread_pool, 10000);

        printf("Time taken to build index:\n");

        auto start = std::chrono::high_resolution_clock::now();

        file_manager.build_index();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        printf("With %d threads: %lld milliseconds\n", threads_num, duration.count());
        thread_pool.terminate();
    }
}


int main() {
//    test();
    ThreadPool thread_pool;
    thread_pool.initialize(EXECUTORS_NUM);

    FileManager file_manager("../data", &thread_pool, 10000);
    file_manager.build_index();
    file_manager.assign_file_watcher();

    Server server(&thread_pool, &file_manager, 8002);
    server.start();

    return 0;
}
