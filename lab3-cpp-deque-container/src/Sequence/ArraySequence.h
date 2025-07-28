#pragma once

#include <stdexcept>
#include <functional>
#include <tuple>
#include "CustomErrors.h"
#include "Sequence.h"
#include "DynamicArray.h"
#include "Iterator.h"

template <typename T>
class ArraySequence: public Sequence<T> {
public:
    DynamicArray<T>* items;
    size_t count;
    size_t capacity;

    virtual ArraySequence<T>* Instance() = 0;

    ArraySequence<T>* AppendInternal(const T& item){
        if (count + 1 == capacity){
            capacity = 2*capacity;
            items->Resize(capacity);
        }
        items->Set(count, item);
        count++;
        return this;    
    }

    Sequence<T>* PrependInternal(const T& item){
        if (count == capacity){
            capacity = 2 * capacity;
            items->Resize(capacity);
        }
        for (int i = count; i > 0; i--){
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
        count++;
        return this;   
    }

    Sequence<T>* DeleteInternal(int index){
        if (index < 0 || index >= count){
            throw IndexOutOfRange;
        }
        items->Delete(index);
        count--;
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& item, int index){
        if (index < 0 || index > count){
            throw IndexOutOfRange;
        }
        if (count == capacity){
            capacity = 2 * capacity;
            items->Resize(capacity);
        }
        for (int i = count; i > index; i--){
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
        count++;
        return this;
    }

    Sequence<T>* ConcatInternal(Sequence<T>* array){
        int size = array->GetLength();
        for (int i = 0; i < size; i++) {
            this->Append(array->Get(i));
        }
        return this;  
    }

public:
    ArraySequence(T* items, int count);
    ArraySequence();
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence();

    virtual T GetFirst() const override;
    virtual T GetLast() const override;
    virtual T& Get(int index) const override;
    virtual ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    T* GetData() const;
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
    virtual T Reduce(std::function<T(const T&, const T&)>, T initial) const override;
    virtual Sequence<T>* Where(std::function<bool(const T&)> f) const override;
    template<typename K>
    Sequence<std::tuple<T, K>>* Zip(ArraySequence<K>& otherList) const;
    template<typename U, typename K>
    std::tuple<Sequence<U>*, Sequence<K>*> Unzip() const;

    void Sort();

    class ArraySequenceIterator: public Iterator<T> {
    private:
        typename DynamicArray<T>::DynamicArrayIterator it;
        friend class ArraySequence;

    public:
        ArraySequenceIterator(): it() {}
        ArraySequenceIterator(const typename DynamicArray<T>::DynamicArrayIterator& it) : it(it) {}

        T& operator*() override { return *it; }
        Iterator<T>& operator++() override { ++it; return *this; }
        Iterator<T>& operator--() override { --it; return *this; }

        bool operator==(const ArraySequenceIterator& other) const { return it == other.it; }
        bool operator!=(const ArraySequenceIterator& other) const { return it != other.it; }
    };

    ArraySequenceIterator erase(ArraySequenceIterator it);

    ArraySequenceIterator begin() { return ArraySequenceIterator(items->begin()); }
    ArraySequenceIterator end() { return ArraySequenceIterator(items->end()); }

};

template<typename T>
class MutableArraySequence: public ArraySequence<T> {
protected:
    ArraySequence<T>* Instance() override {
        return this;
    }
 public:
    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(const ArraySequence<T>& otherArray) : ArraySequence<T>(otherArray) {}
};


template <typename T>
const T& ArraySequence<T>::operator[](int index) const {
    if (index < 0 || index >= count){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
T& ArraySequence<T>::operator[](int index){
    if (index < 0 || index >= count){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
bool ArraySequence<T>::operator==(const Sequence<T>& otherList){
    if (this->count != otherList.GetLength()){ return false; }
    for (int i = 0; i < count; ++i){
        if ((*this)[i] != otherList[i]){ return false; }
    }
    return true;
}

template <typename T>
bool ArraySequence<T>::operator!=(const Sequence<T>& otherList){
    if (this->count != otherList.GetLength()){ return true; }
    for (int i = 0; i < count; ++i){
        if ((*this)[i] != otherList[i]){ return true; }
    }
    return false;
}

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int count){
    if (count < 0){
        throw NotValidArgument;
    }
    this->capacity = (count > 0) ? (2 * count) : 1;
    this->count = count;    
    this->items = new DynamicArray<T>(items, count);
}

template <typename T>
ArraySequence<T>::ArraySequence(){
    this->items = new DynamicArray<T>(0);
    this->count = 0;
    this->capacity = 1;
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& array){
    this->count = array.count;
    this->capacity = array.capacity;
    this->items = new DynamicArray<T>(*array.items);
}

template <typename T>
ArraySequence<T>::~ArraySequence(){
    delete items;
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    if (count == 0){
        throw EmptyContainer;
    }
    return items->Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    if (count == 0){
        throw EmptyContainer;
    }
    return items->Get(count - 1);
}

template <typename T>
T& ArraySequence<T>::Get(int index) const {
    if (index < 0 || index >= count){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
int ArraySequence<T>::GetLength() const {
    return count;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Delete(int index){
    return Instance()->DeleteInternal(index);
}

template <typename T>
T* ArraySequence<T>::GetData() const {
    return this->items->GetData();
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= count || endIndex < startIndex){
        throw IndexOutOfRange;
    }
    int len = endIndex - startIndex + 1;
    T* data = new T[len];
    for (int i = 0; i < len; ++i){
        data[i] = items->Get(startIndex + i);
    }
    MutableArraySequence<T>* subSeq = new MutableArraySequence<T>(data, len);
    delete[] data;
    return subSeq;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Append(const T& item){
    return Instance()->AppendInternal(item);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Prepend(const T& item){
    return Instance()->PrependInternal(item);
}

template <typename T>
Sequence<T>* ArraySequence<T>::InsertAt(const T& item, int index){
    return Instance()->InsertAtInternal(item, index);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* array){
    return Instance()->ConcatInternal(array);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Map(std::function<T(const T&)> f) const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < count; ++i){
        result->Append(f(items->Get(i)));
    }
    return result;
}

template <typename T>
T ArraySequence<T>::Reduce(std::function<T(const T&, const T&)> f, T initial) const {
    T result = initial;
    for (int i = 0; i < count; ++i){
        result = f(result, items->Get(i));
    }
    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Where(std::function<bool(const T&)> f) const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (int i = 0; i < count; ++i){
        T cur = items->Get(i);
        if (f(cur)){
            result->Append(cur);
        }
    }
    return result;
}

template<typename T>
template<typename K>
Sequence<std::tuple<T, K>>* ArraySequence<T>::Zip(ArraySequence<K>& otherList) const {
    MutableArraySequence<std::tuple<T, K>>* result = new MutableArraySequence<std::tuple<T, K>>();
    int size = (count < otherList->GetLength()) ? count : otherList->GetLength();
    for (int i = 0; i < size; ++i){
        result->Append(std::make_tuple(this->Get(i), otherList->Get(i)));
    }
    return result;
}

template<typename T>
template<typename U, typename K>
std::tuple<Sequence<U>*, Sequence<K>*> ArraySequence<T>::Unzip() const {
    static_assert(std::is_same<T, std::tuple<U, K>>::value);
    
    MutableArraySequence<T>* firstSeq = new MutableArraySequence<T>();
    MutableArraySequence<K>* secondSeq = new MutableArraySequence<K>();

    for (int i = 0; i < count; ++i) {
        T tuple = items->Get(i);
        firstSeq->Append(std::get<0>(tuple));
        secondSeq->Append(std::get<1>(tuple));
    }

    return std::make_tuple(firstSeq, secondSeq);
}

template <typename T>
typename ArraySequence<T>::ArraySequenceIterator ArraySequence<T>::erase(ArraySequenceIterator it){
    ArraySequenceIterator new_it = ArraySequenceIterator(items->erase(it.it));
    this->count--;
    return new_it;
}

template<typename T>
void ArraySequence<T>::Sort(){
    items->sort();
}
