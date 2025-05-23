#ifndef LIST_SEQUENCE_ARRAY_H
#define LIST_SEQUENCE_ARRAY_H

#include <stdexcept>
#include <functional>
#include "Sequence.h"
#include "LinkedList.h"
// #include "MutableListSequence.h"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* items;

    virtual ListSequence<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& item) {
        items->Append(item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& item) {
        items->Prepend(item);
        return this;
    }

    Sequence<T>* DeleteInternal(int index){
        items->Delete(index);
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& item, int index) {
        items->InsertAt(item, index);
        return this;
    }

    Sequence<T>* ConcatInternal(Sequence<T>* list) {
        int size = list->GetLength();
        for (int i = 0; i < size; i++) {
            items->Append(list->Get(i));
        }
        return this;
    }

public: 
    ListSequence(T* items, int count);
    ListSequence();
    ListSequence(const LinkedList<T>* otherList);
    virtual ~ListSequence();

    virtual T GetFirst() const override;
    virtual T GetLast() const override;
    virtual T& Get(int index) const override;
    virtual ListSequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    virtual int GetLength() const override;
    virtual Sequence<T>* Delete(int index) override;

    virtual Sequence<T>* Append(const T& item) override;
    virtual Sequence<T>* Prepend(const T& item) override;
    virtual Sequence<T>* InsertAt(const T& item, int index) override;
    virtual Sequence<T>* Concat(Sequence<T>* list) override;

    virtual const T& operator[](int index) const override;
    virtual T& operator[](int index) override;
    virtual bool operator==(const Sequence<T>& otherList) override;
    virtual bool operator!=(const Sequence<T>& otherList) override;

    virtual Sequence<T>* Map(std::function<T(const T&)> f) const override;
    virtual T Reduce(std::function<T(const T&, const T&)> f, T initial) const override;
    virtual Sequence<T>* Find(std::function<bool(const T&)> f) const override;
    template<typename K>
    Sequence<std::tuple<T, K>>* Zip(ListSequence<K>& otherList) const;
    template<typename K>
    std::tuple<Sequence<T>*, Sequence<K>*> Unzip() const;

    class Iterator {
    private:
        const ListSequence<T>* data;
        size_t index;
    public:
        Iterator(const ListSequence<T>* data, size_t index) : data(data), index(index) {}

        T& Get(){return data->Get(index); }
        void Next() { index++; }

        T& operator*(){ return data->Get(index); }
        Iterator& operator++() { index++; return *this; }
        bool operator!=(const Iterator& other ){ return index != other.index; }
        bool operator==(const Iterator& other) const { return index == other.index; }
    };

    Iterator begin() const { return Iterator(this, 0); };
    Iterator end() const { return Iterator(this, this->GetLength()); };
};

template <typename T>
class MutableListSequence : public ListSequence<T> {
public:
    ListSequence<T>* Instance() override {
        return this; 
    }
public:
    MutableListSequence(T* data, int count) : ListSequence<T>(data, count) {}
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(const LinkedList<T>* other) : ListSequence<T>(other) {}
};

template <typename T>
const T& ListSequence<T>::operator[](int index) const {
    if (index < 0 || index >= this->GetLength()){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
T& ListSequence<T>::operator[](int index){
    if (index < 0 || index >= this->GetLength()){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
bool ListSequence<T>::operator==(const Sequence<T>& otherList){
    if (this->GetLength() != otherList.GetLength()) {
        return false;
    }
    for (int i = 0; i < this->GetLength(); ++i) {
        if (this->Get(i) != otherList.Get(i)) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool ListSequence<T>::operator!=(const Sequence<T>& otherList){
    if (this->GetLength() != otherList.GetLength()) {
        return true;
    }
    for (int i = 0; i < this->GetLength(); ++i) {
        if (this->Get(i) != otherList.Get(i)) {
            return true;
        }
    }
    return false;
}

template<typename T>
ListSequence<T>::ListSequence(T* items, int count){
    this->items = new LinkedList<T>(items, count);
}

template<typename T>
ListSequence<T>::ListSequence(){
    items = new LinkedList<T>();
}

template<typename T>
ListSequence<T>::ListSequence(const LinkedList<T>* otherList){
    this->items = new LinkedList<T>(otherList);
}

template<typename T>
ListSequence<T>::~ListSequence(){
    delete items;
}

template<typename T>
T ListSequence<T>::GetFirst() const{
    return items->GetFirst();
}

template<typename T>
T ListSequence<T>::GetLast() const {
    return items->GetLast();
}

template<typename T>
T& ListSequence<T>::Get(int index) const {
    return items->Get(index);
}

template<typename T>
ListSequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    ListSequence<T>* subSeq = new MutableListSequence<T>();
    subSeq->items = this->items->GetSubList(startIndex, endIndex);
    return subSeq;
}

template<typename T>
int ListSequence<T>::GetLength() const {
    return items->GetLength();
}

template<typename T>
Sequence<T>* ListSequence<T>::Append(const T& item) {
    return Instance()->AppendInternal(item);
}

template <typename T>
Sequence<T>* ListSequence<T>::Delete(int index){
    return Instance()->DeleteInternal(index);
}

template<typename T>
Sequence<T>* ListSequence<T>::Prepend(const T& item) {
    return Instance()->PrependInternal(item);
}

template<typename T>
Sequence<T>* ListSequence<T>::InsertAt(const T& item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* otherList) {
    return Instance()->ConcatInternal(otherList);
}

template<typename T>
Sequence<T>* ListSequence<T>::Map(std::function<T(const T&)> f) const {
    MutableListSequence<T>* result = new MutableListSequence<T>();
    for (int i = 0; i < this->GetLength(); ++i){
        result->Append(f(this->Get(i)));
    }
    return result;
}

template<typename T>
T ListSequence<T>::Reduce(std::function<T(const T&, const T&)> f, T initial) const {
    T result = initial;
    for (int i = 0; i < this->GetLength(); ++i){
        result = f(result, this->Get(i));
    }
    return result;
}

template<typename T>
Sequence<T>* ListSequence<T>::Find(std::function<bool(const T&)> f) const {
    MutableListSequence<T>* result = new MutableListSequence<T>();
    for (int i = 0; i < this->GetLength(); ++i){
        T cur = this->Get(i);
        if(f(cur)){
            result->Append(cur);
        }
    }
    return result;
}

template<typename T>
template<typename K>
Sequence<std::tuple<T, K>>* ListSequence<T>::Zip(ListSequence<K>& otherList) const {
    MutableListSequence<std::tuple<T, K>>* result = new MutableListSequence<std::tuple<T, K>>();
    int size = (this->GetLength < otherList->GetLength()) ? this->GetLength() : otherList->GetLength();
    for (int i = 0; i < size; ++i){
        result->Append(std::make_tuple(this->Get(i), otherList->Get(i)));
    }
    return result;
}

template<typename T>
template<typename K>
std::tuple<Sequence<T>*, Sequence<K>*> ListSequence<T>::Unzip() const {
    if(!std::is_same<T, std::tuple<T, K>>::value){
        throw NotValidArgument;
    }

    MutableListSequence<T>* firstSeq = new MutableListSequence<T>();
    MutableListSequence<K>* secondSeq = new MutableListSequence<K>();

    for (int i = 0; i < this->GetLength(); ++i) {
        T tuple = items->Get(i);
        firstSeq->Append(std::get<0>(tuple));
        secondSeq->Append(std::get<1>(tuple));
    }

    return std::make_tuple(firstSeq, secondSeq);
}

#endif