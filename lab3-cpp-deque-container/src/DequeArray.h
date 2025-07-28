#pragma once

#include "Sequence\ArraySequence.h"
#include "Deque.h"
#include "Sequence\Iterator.h"

template <typename T>
class DequeArray: public Deque<T> {
private:
    MutableArraySequence<T>* data;
public:
    DequeArray();
    DequeArray(T* data, size_t size);
    ~DequeArray();

    T front() override;
    T back() override;
    void push_back(const T& value) override;
    void push_front(const T& value) override;
    void pop_back() override;
    void pop_front() override;
    bool empty() override;
    void sort() override;
    size_t length() override;
    DequeArray<T>* get_sub(int begin, int end) override;
    void concat(DequeArray<T>& other_deq);

    void map(std::function<T(const T&)> f) override;
    T reduce(std::function<T(const T&, const T&)> f, T initial) override;
    DequeArray<T>* where(std::function<bool(const T&)> f) override;

    class DequeArrayIterator: public Iterator<T> {
    private:
        typename ArraySequence<T>::ArraySequenceIterator it;
        friend class DequeArray;

    public:
        DequeArrayIterator() : it() {}
        DequeArrayIterator(const typename ArraySequence<T>::ArraySequenceIterator& it) : it(it) {}

        typename ArraySequence<T>::ArraySequenceIterator getIt() const {
            return it;
        }

        T& operator*()  { return *it; }
        Iterator<T>& operator++() override { ++it; return *this; }
        Iterator<T>& operator--() override { --it; return *this; }

        bool operator==(const DequeArrayIterator& other) const { return it == other.it; }
        bool operator!=(const DequeArrayIterator& other) const { return it != other.it; }
    };

    typename DequeArray<T>::DequeArrayIterator erase(typename DequeArray<T>::DequeArrayIterator it);
    void clear(typename DequeArray<T>::DequeArrayIterator beginIt, typename DequeArray<T>::DequeArrayIterator endIt);
    typename DequeArray<T>::DequeArrayIterator search(DequeArray<T>& deq);

    typename DequeArray<T>::DequeArrayIterator begin() { return typename DequeArray<T>::DequeArrayIterator(data->begin()); }
    typename DequeArray<T>::DequeArrayIterator end() { return typename DequeArray<T>::DequeArrayIterator(data->end()); }

    
};

template <typename T>
DequeArray<T>::DequeArray(){
    this->data = new MutableArraySequence<T>();
}

template <typename T>
DequeArray<T>::DequeArray(T* items, size_t size){
    this->data = new MutableArraySequence<T>(items, size);
}

template <typename T>
DequeArray<T>::~DequeArray(){
    delete data;
}

template <typename T>
T DequeArray<T>::front(){
    return data->GetFirst();
}

template <typename T>
T DequeArray<T>::back(){
    return data->GetLast();
}

template <typename T>
void DequeArray<T>::push_front(const T& value){
    data->Prepend(value);
}

template <typename T>
void DequeArray<T>::push_back(const T& value){
    data->Append(value);
}

template <typename T>
void DequeArray<T>::pop_front(){
    data->Delete(0);
}

template <typename T>
void DequeArray<T>::pop_back(){
    data->Delete(data->GetLength()-1);
}

template <typename T>
typename DequeArray<T>::DequeArrayIterator DequeArray<T>::erase(typename DequeArray<T>::DequeArrayIterator it){
    typename DequeArray<T>::DequeArrayIterator new_it = typename DequeArray<T>::DequeArrayIterator(data->erase(it.getIt()));
    return new_it;
}

template <typename T>
bool DequeArray<T>::empty(){
    return data->GetLength() == 0;
}

template <typename T>
size_t DequeArray<T>::length(){
    return data->GetLength();
}

template <typename T>
void DequeArray<T>::sort(){
    data->Sort();
}

template<typename T>
void DequeArray<T>::clear(typename DequeArray<T>::DequeArrayIterator beginIt, typename DequeArray<T>::DequeArrayIterator endIt) {
        typename DequeArray<T>::DequeArrayIterator current_iterator = beginIt;
        while (current_iterator != endIt) {
            if (this->empty()) { 
                break;
            }
            current_iterator = erase(current_iterator);
        }
}

template <typename T>
void DequeArray<T>::map(std::function<T(const T&)> f){
    Sequence<T>* seq = data->Map(f);
    clear(begin(), end());
    for (int i = 0; i < seq->GetLength(); ++i){
        data->Append(seq->Get(i));
    }
    delete seq;
}

template <typename T>
T DequeArray<T>::reduce(std::function<T(const T&, const T&)> f, T initial){
    return data->Reduce(f, initial);
}

template <typename T>
DequeArray<T>* DequeArray<T>::where(std::function<bool(const T&)> f){
    Sequence<T>* seq = data->Where(f);
    DequeArray<T>* new_deq = new DequeArray<T>();
    for (int i = 0; i < seq->GetLength(); ++i){
        new_deq->push_back(seq->Get(i));
    }
    delete seq;
    return new_deq;
}

template <typename T>
DequeArray<T>* DequeArray<T>::get_sub(int begin, int end){
    Sequence<T>* seq = data->GetSubsequence(begin, end);
    DequeArray<T>* new_deq = new DequeArray<T>();
    for (int i = 0; i < seq->GetLength(); ++i){
        new_deq->push_back(seq->Get(i));
    }
    delete seq;
    return new_deq;
}

template <typename T>
void DequeArray<T>::concat(DequeArray<T>& other_deq){
    data->Concat(other_deq.data);
}

template <typename T>
typename DequeArray<T>::DequeArrayIterator DequeArray<T>::search(DequeArray<T>& deq) {
    if (deq.empty()) return this->begin();
    if (this->length() < deq.length()) return this->end();

    auto main_it_end = this->end();
    auto sub_it_end = deq.end();

    for (auto main_it = this->begin(); main_it != main_it_end; ++main_it) {
        auto tmp = main_it;
        auto sub_it = deq.begin();

        while (sub_it != sub_it_end && tmp != main_it_end && *tmp == *sub_it) {
            ++tmp;
            ++sub_it;
        }

        if (sub_it == sub_it_end) {
            return main_it;
        }
    }

    return this->end();
}
