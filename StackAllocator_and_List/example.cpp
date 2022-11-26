/*
 * Это файл лишь пример. Здесь не тестируется
 * большинство функций, а также не проверяется
 * работа аллокатора. Однако все это было проверено
 * в системе Яндекс.Контест в контесте от Ильи Мещерина
 * читающего курс в МФТИ, в рамках которого и была
 * написана данная программа.
 */

#include <iostream>
#include <list>
#include <ctime>
#include "List.h"
#include "StackAllocator.h"

void simple_list_test() {
    List<int> my_list;
    std::list<int> std_list;

    for (int i = 0; i < 1000; ++i) {
        my_list.push_back(i);
        std_list.push_back(i);
    }

    if (my_list.size() == std_list.size()) {
        std::cout << "simple_list_test: OK" << std::endl;
    } else {
        std::cout << "simple_list_test: FAIL" << std::endl;
    }
}

void perfomance_list_test() {
    List<int> my_list;
    std::list<int> std_list;

    clock_t start = clock();
    for (int i = 0; i < 1000000; ++i) {
        my_list.push_back(i);
    }
    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    std::cout << "my_list.push_back: " << time << std::endl;

    start = clock();
    for (int i = 0; i < 1000000; ++i) {
        std_list.push_back(i);
    }
    end = clock();
    time = (double) (end - start) / CLOCKS_PER_SEC;
    std::cout << "std_list.push_back: " << time << std::endl;
}

int main() {
    simple_list_test();
    perfomance_list_test();
    return 0;
}