#pragma once

#include "Sequence\ListSequence.h"
#include "Deque.h"
#include "Sequence\Iterator.h"

template <typename T>
class DequeList: public Deque<T> {
private:
    MutableListSequence<T>* data;
public:
    DequeList();
    DequeList(T* items, size_t size);
    ~DequeList();

    //Стандартные операции
    T front() override;
    T back() override;
    void push_back(const T& value) override;
    void push_front(const T& value) override;
    void pop_back() override;
    void pop_front() override;
    bool empty() override;
    void sort() override;
    size_t length() override;
    DequeList<T>* get_sub(int begin, int end) override;
    void concat(DequeList<T>& other_deq);

    void map(std::function<T(const T&)> f) override;
    T reduce(std::function<T(const T&, const T&)> f, T initial) override;
    DequeList<T>* where(std::function<bool(const T&)> f) override;

    class DequeListIterator: public Iterator<T> {
    private:
        typename ListSequence<T>::ListSequenceIterator it;
        friend class DequeList;

    public:
        DequeListIterator(): it() {}
        DequeListIterator(const typename ListSequence<T>::ListSequenceIterator& it) : it(it) {}

        typename ListSequence<T>::ListSequenceIterator getIt() const {
            return it;
        }

        T& operator*() override { return *it; }
        Iterator<T>& operator++() override { ++it; return *this; }
        Iterator<T>& operator--() override { --it; return *this; }
      
        bool operator==(const DequeListIterator& other) const  { return it == other.it; }
        bool operator!=(const DequeListIterator& other) const  { return it != other.it; }
    };

    typename DequeList<T>::DequeListIterator erase(typename DequeList<T>::DequeListIterator it);
    void clear(typename DequeList<T>::DequeListIterator beginIt, typename DequeList<T>::DequeListIterator endIt);
    typename DequeList<T>::DequeListIterator search(DequeList<T>& deq);

    typename DequeList<T>::DequeListIterator begin() { return typename DequeList<T>::DequeListIterator(data->begin()); }
    typename DequeList<T>::DequeListIterator end() { return typename DequeList<T>::DequeListIterator(data->end()); }

};
template <typename T>
DequeList<T>::DequeList(){
    this->data = new MutableListSequence<T>();
}

template <typename T>
DequeList<T>::DequeList(T* items, size_t size){
    this->data = new MutableListSequence<T>(items, size);
}

template <typename T>
DequeList<T>::~DequeList(){
    delete data;
}

template <typename T>
T DequeList<T>::front(){
    return data->GetFirst();
}

template <typename T>
T DequeList<T>::back(){
    return data->GetLast();
}

template <typename T>
void DequeList<T>::push_front(const T& value){
    data->Prepend(value);
}

template <typename T>
void DequeList<T>::push_back(const T& value){
    data->Append(value);
}

template <typename T>
void DequeList<T>::pop_front(){
    data->Delete(0);
}

template <typename T>
void DequeList<T>::pop_back(){
    data->Delete(data->GetLength()-1);
}

template <typename T>
typename DequeList<T>::DequeListIterator DequeList<T>::erase(typename DequeList<T>::DequeListIterator it){
    typename DequeList<T>::DequeListIterator new_it = typename DequeList<T>::DequeListIterator(data->erase(it.getIt()));
    return new_it;
}

template <typename T>
bool DequeList<T>::empty(){
    return data->GetLength() == 0;
}

template <typename T>
size_t DequeList<T>::length(){
    return data->GetLength();
}

template <typename T>
void DequeList<T>::sort(){
    data->Sort();
}

template<typename T>
void DequeList<T>::clear(typename DequeList<T>::DequeListIterator beginIt, typename DequeList<T>::DequeListIterator endIt) {
    while (beginIt != endIt) {
        beginIt = erase(beginIt);
    }
}

template <typename T>
void DequeList<T>::map(std::function<T(const T&)> f){
    Sequence<T>* seq = data->Map(f);
    clear(begin(), end());
    for (int i = 0; i < seq->GetLength(); ++i){
        data->Append(seq->Get(i));
    }
    delete seq;
}

template <typename T>
T DequeList<T>::reduce(std::function<T(const T&, const T&)> f, T initial){
    return data->Reduce(f, initial);
}

template <typename T>
DequeList<T>* DequeList<T>::where(std::function<bool(const T&)> f){
    Sequence<T>* seq = data->Where(f);
    DequeList<T>* new_deq = new DequeList<T>();
    for (int i = 0; i < seq->GetLength(); ++i){
        new_deq->push_back(seq->Get(i));
    }
    delete seq;
    return new_deq;
}

template <typename T>
DequeList<T>* DequeList<T>::get_sub(int begin, int end){
    Sequence<T>* seq = data->GetSubsequence(begin, end);
    DequeList<T>* new_deq = new DequeList<T>();
    for (int i = 0; i < seq->GetLength(); ++i){
        new_deq->push_back(seq->Get(i));
    }
    delete seq;
    return new_deq;
}

template <typename T>
void DequeList<T>::concat(DequeList<T>& other_deq){
    data->Concat(other_deq.data);
}

template <typename T>
typename DequeList<T>::DequeListIterator DequeList<T>::search(DequeList<T>& deq) {
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