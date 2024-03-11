#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <functional>
#include "skip_list.h"

// 线程数量
constexpr int NUM_THREADS = 1;

// 执行操作总数
constexpr int TEST_COUNT = 100000;

SkipList<int, std::string> test_skip_list(17);

int GetRandomNumber(std::mt19937& gen) {
    std::uniform_int_distribution<int> distrib(0, 99);
    return distrib(gen);
}

void TestInsertElement(int tid, std::mt19937& gen) {
    std::cout << tid << std::endl;
    int count_each_thread = TEST_COUNT / NUM_THREADS;
    for (int i = tid * count_each_thread, count = 0; count < count_each_thread;
         i++) {
        count++;
        test_skip_list.InsertElement(GetRandomNumber(gen) % TEST_COUNT, "test");
    }
}

void TestGetElement(int tid, std::mt19937& gen) {
    std::cout << tid << std::endl;
    int count_each_thread = TEST_COUNT / NUM_THREADS;
    for (int i = tid * count_each_thread, count = 0; count < count_each_thread;
         i++) {
        count++;
        test_skip_list.SearchElement(GetRandomNumber(gen) % TEST_COUNT);
    }
}

int main() {
    auto seed =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    std::chrono::duration<double> insert_elapsed, search_elapsed;

    {
        std::vector<std::thread> threads(NUM_THREADS);
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            threads.emplace_back(TestInsertElement, i, std::ref(gen));
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        insert_elapsed = finish - start;
        std::cout << "test insert element end." << std::endl;
    }

    test_skip_list.PrintList();

    {
        std::vector<std::thread> threads(NUM_THREADS);
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            threads.emplace_back(TestGetElement, i, std::ref(gen));
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        search_elapsed = finish - start;
        std::cout << "test search element end." << std::endl;
    }

    std::cout << "insert elapsed:" << insert_elapsed.count() << std::endl;
    std::cout << "search elapsed:" << search_elapsed.count() << std::endl;

    return 0;
}
