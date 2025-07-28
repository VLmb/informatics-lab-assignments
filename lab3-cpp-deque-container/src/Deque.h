#pragma once

#include "Sequence\Iterator.h"
#include "functional"

template <typename T>
class Deque {
public:
    virtual T front() = 0;
    virtual T back() = 0;
    virtual void push_back(const T& value) = 0;
    virtual void push_front(const T& value) = 0;
    virtual void pop_back() = 0;
    virtual void pop_front() = 0;
    virtual bool empty() = 0;
    virtual void sort() = 0;
    virtual size_t length() = 0;
    virtual Deque<T>* get_sub(int begin, int end) = 0;
    virtual void map(std::function<T(const T&)> f) = 0;
    virtual T reduce(std::function<T(const T&, const T&)> f, T initial) = 0;
    virtual Deque<T>* where(std::function<bool(const T&)> f) = 0;

};
