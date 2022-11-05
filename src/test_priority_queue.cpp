#include <map>
#include <vector>

#include <catch.hpp>

#include "priority_queue.h"

TEST_CASE("PriorityQueueSorting") {
    std::vector<int> elements = {1, 5, 2, 3, 7, 6, 4};
    std::vector<int> expected = {7, 6, 5, 4, 3, 2, 1};

    {  // TopPop
        PriorityQueue<int> priority_queue;
        for (auto el : elements) {
            priority_queue.Push(el);
        }
        std::vector<int> sorted_elements = {};
        while (!priority_queue.Empty()) {
            sorted_elements.push_back(priority_queue.TopPop());
        }
        REQUIRE(sorted_elements == expected);
    }

    {  // Top, Pop
        PriorityQueue<int> priority_queue;
        for (auto el : elements) {
            priority_queue.Push(el);
        }
        std::vector<int> sorted_elements = {};
        while (!priority_queue.Empty()) {
            sorted_elements.push_back(priority_queue.Top());
            priority_queue.Pop();
        }
        REQUIRE(sorted_elements == expected);
    }
}

TEST_CASE("PriorityQueueSortingComp") {
    std::vector<int> elements = {1, 5, 2, 3, 7, 6, 4};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7};

    PriorityQueue<int, std::greater<>> priority_queue;
    for (auto el : elements) {
        priority_queue.Push(el);
    }
    std::vector<int> sorted_elements = {};
    while (!priority_queue.Empty()) {
        sorted_elements.push_back(priority_queue.TopPop());
    }
    REQUIRE(sorted_elements == expected);
}

TEST_CASE("PriorityQueueSizing") {
    std::vector<int> elements = {1, 5, 2, 3, 7, 6, 4};

    PriorityQueue<int> priority_queue;
    REQUIRE(priority_queue.Size() == 0);
    REQUIRE(priority_queue.Empty());
    for (size_t i = 0; i < elements.size(); ++i) {
        priority_queue.Push(elements[i]);
        REQUIRE(priority_queue.Size() == i + 1);
        REQUIRE(!priority_queue.Empty());
    }
    for (size_t i = 0; i < elements.size(); ++i) {
        priority_queue.Pop();
        REQUIRE(priority_queue.Size() == elements.size() - i - 1);
        if (i != elements.size() - 1) {
            REQUIRE(!priority_queue.Empty());
        }
    }
    REQUIRE(priority_queue.Empty());
}
