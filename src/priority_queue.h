#pragma once

#include <functional>
#include <vector>

#include "heap.h"

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    using Container = std::vector<T>;

    PriorityQueue() : comp_() {
    }

    explicit PriorityQueue(const Compare& comp) : comp_(comp) {
    }

    void Push(T value) {
        heap_.push_back(std::move(value));
        PushHeap(heap_.begin(), heap_.end(), comp_);
    }
    T Top() const {
        return heap_.front();
    }
    void Pop() {
        PopHeap(heap_.begin(), heap_.end(), comp_);
        heap_.pop_back();
    }
    T TopPop() {
        PopHeap(heap_.begin(), heap_.end(), comp_);
        T res = std::move(heap_.back());
        heap_.pop_back();
        return res;
    }
    auto Size() {
        return heap_.size();
    }
    bool Empty() {
        return heap_.empty();
    }

private:
    Container heap_;

    Compare comp_;
};
