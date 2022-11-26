#include <iostream>
#include <deque>
#include "deque.h"

void simple_test() {
    Deque<int> my_d;
    for (int i = 0; i < 10; i++) {
        my_d.push_back(i);
    }
    std::deque<int> d;
    for (int i = 0; i < 10; i++) {
        d.push_back(i);
    }

    bool ok = true;
    for (int i = 0; i < 10; i++) {
        if (my_d[i] != d[i]) {
            ok = false;
            break;
        }
    }

    if (ok) {
        std::cout << "Simple deque test passed" << std::endl;
    } else {
        std::cout << "Simple deque test failed" << std::endl;
    }
}

void push_pop_test() {
    Deque<int> my_d;
    std::deque<int> d;
    for (int i = 0; i < 10; i++) {
        my_d.push_back(i);
        d.push_back(i);
    }
    for (int i = 0; i < 10; i++) {
        my_d.push_front(i);
        d.push_front(i);
    }
    for (int i = 0; i < 10; i++) {
        my_d.pop_back();
        d.pop_back();
    }
    for (int i = 0; i < 10; i++) {
        my_d.pop_front();
        d.pop_front();
    }

    bool ok = true;
    for (int i = 0; i < 10; i++) {
        if (my_d[i] != d[i]) {
            ok = false;
            break;
        }
    }

    if (ok) {
        std::cout << "Push/pop test passed" << std::endl;
    } else {
        std::cout << "Push/pop test failed" << std::endl;
    }
}

void size_test() {
    Deque<int> my_d;
    std::deque<int> d;
    for (int i = 0; i < 10; i++) {
        my_d.push_back(i);
        d.push_back(i);
    }
    for (int i = 0; i < 10; i++) {
        my_d.push_front(i);
        d.push_front(i);
    }
    for (int i = 0; i < 10; i++) {
        my_d.pop_back();
        d.pop_back();
    }
    for (int i = 0; i < 10; i++) {
        my_d.pop_front();
        d.pop_front();
    }

    bool ok = true;
    if (my_d.size() != d.size()) {
        ok = false;
    }

    if (ok) {
        std::cout << "Size test passed" << std::endl;
    } else {
        std::cout << "Size test failed" << std::endl;
    }
}

int main() {
    simple_test();
    push_pop_test();
    size_test();
    return 0;
}
