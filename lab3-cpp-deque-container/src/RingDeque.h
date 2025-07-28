#pragma once

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Deque.h"
#include "functional"
#include "Sequence/DynamicArray.h"

template <typename T>
class RingDeque: public Deque<T> {
private:
    DynamicArray<T>* data;
    size_t count;
    size_t capacity;
    size_t first;
    size_t last;

    void grow(){
        DynamicArray<T>* new_data = new DynamicArray<T>(capacity * 2);
        size_t ptr = first;
        for (int i = 0; i < count; ++i) {
            new_data->Set(i, data->Get(ptr % capacity));
            ptr++;
        }
        delete data;
        data = new_data;
        first = 0;
        last = count;
        capacity = capacity * 2;
    }

public:
    //Constructors
    RingDeque(): count(0), capacity(1), first(0), last(0) {
        data = new DynamicArray<T>(1);
    }
    RingDeque(size_t size): count(0), capacity(size), first(0), last(0) {
        data = new DynamicArray<T>(size);
    }
    RingDeque(T* items, int count) {
        this->count = count;
        this->capacity = count*2;
        data = new DynamicArray<T>(items, count);
        data->Resize(capacity);
        this->first = 0;
        this->last = count;
    }

    ~RingDeque() {
        delete data;
    }

    //Operations
    T front() override {
        if (count == 0) throw EmptyContainer;
        return data->Get(first);
    }

    T back() override {
        if (count == 0) throw EmptyContainer;
        return data->Get((last - 1) % capacity);
    }

    void push_back(const T& value) override {
        if (count == capacity - 1) {
            this->grow();
        }
        data->Set(last, value);
        last = (last + 1) % capacity;
        count++;
    }

    void push_front(const T& value) override {
        if (count == capacity - 1) {
            this->grow();
        }
        first = (first - 1 + capacity) % capacity;
        data->Set(first, value);
        count++;
    }

    void pop_back() override {
        if (count == 0){
            throw EmptyContainer;
        }
        last = (last - 1 + capacity) % capacity;
        count--;
    }

    void pop_front() override {
        if (count == 0){
            throw EmptyContainer;
        }
        first = (first + 1) % capacity;
        count--;
    }

    size_t length() override {
        return count;
    }

    Deque<T>* get_sub(int begin, int end) override {
        if (begin > end || end > count || begin < 0 || end < 0) throw IndexOutOfRange;
        RingDeque<T>* sub_deq = new RingDeque<T>(end - begin);
        for (int i = begin; i < end; ++i) {
            sub_deq->push_back(data->Get((i + first) % capacity));
        }
        return sub_deq;
    }

    bool empty() override {
        return count == 0;
    }

    void sort() override {
        DynamicArray<T>* sort_data = new DynamicArray<T>(count);
        for (int i = 0; i < count; ++i){
            sort_data->Set(i, data->Get((first + i) % capacity));
        }
        sort_data->sort();
        sort_data->Resize(count*2);
        delete data;
        data = sort_data;
        this->first = 0;
        this->last = count;
        this->capacity = count*2;
    }

    void map(std::function<T(const T&)> f) override {
        size_t ptr = first;
        for (int i = 0; i < count; ++i) {
            data->Set(ptr % capacity, f(data->Get(ptr % capacity)));
            ptr++;
        }
    }

    T reduce(std::function<T(const T&, const T&)> f, T initial) override {
        size_t ptr = first;
        T result = initial;
        for (int i = 0; i < count; ++i) {
            result = f(result, data->Get(ptr % capacity));
            ptr++;
        }
        return result;
    };

    Deque<T>* where(std::function<bool(const T&)> f) override {
        RingDeque<T>* new_dq = new RingDeque<T>();
        size_t ptr = first;
        T tmp;
        for (int i = 0; i < count; ++i) {
            tmp = data->Get(ptr % capacity);
            if (f(tmp)){
                new_dq->push_back(tmp);
            }
            ptr++;
        }
        return new_dq;
    }

    void concat(RingDeque<T>& other_deq) {
        size_t len = other_deq.length();
        DynamicArray<T> items(len);
        for (size_t i = 0; i < len; ++i){
            items.Set(i, other_deq.data->Get((other_deq.first + i) % other_deq.capacity));
        }
        for (size_t i = 0; i < len; ++i) {
           this->push_back(items.Get(i));
        }
    }

    RingDeque<T>& operator=(const RingDeque<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete data;

        count = other.count;
        capacity = other.capacity;
        first = other.first;
        last = other.last;
        data = new DynamicArray<T>(*other.data);
        return *this;
    }

    void specialPrint() {
        DynamicArray<T>& arr = *data;
        size_t size = arr.GetSize();
        const int cellWidth = 5;

        std::cout << "Array:\n";

        for (size_t i = 0; i < size; ++i) {
            std::cout << std::setw(cellWidth) << arr.Get(i);
        }
        std::cout << '\n';

        for (size_t i = 0; i < size; ++i) {
            std::cout << std::setw(cellWidth) << i;
        }
        std::cout << '\n';

        bool flag = false;
        for (size_t i = 0; i < size; ++i) {
            if (i == first && i == last)
                std::cout << std::setw(cellWidth) << "F/L";
            else if (i == first)
                std::cout << std::setw(cellWidth) << " F ";
            else if (i == last) {
                flag = true;
                std::cout << std::setw(cellWidth) << " L ";
            }
            else
                std::cout << std::setw(cellWidth) << " ";
        }
        if (flag == false) std::cout << std::setw(cellWidth) << "L";
        std::cout << '\n';
    }

    class RingDequeIterator: public Iterator<T> {
    private:
        RingDeque<T>* owner;
        size_t ptr;
        friend class RingDeque;

    public:
        RingDequeIterator(RingDeque<T>* owner, size_t ptr): owner(owner), ptr(ptr) {}

        T& operator*() override {
            return owner->data->Get((owner->first + ptr) % owner->capacity);
        }

        RingDequeIterator& operator++() override {
            ++ptr;
            return *this;
        }

        RingDequeIterator& operator--() override {
            --ptr;
            return *this;
        }

        bool operator!=(const RingDequeIterator& other) const {
            return ptr != other.ptr || owner != other.owner;
        }

        bool operator==(const RingDequeIterator& other) const {
            return !(ptr != other.ptr || owner != other.owner);
        }
    };

    RingDequeIterator begin() { return RingDequeIterator(this, 0); }
    RingDequeIterator end() { return RingDequeIterator(this, count); }
        
};



