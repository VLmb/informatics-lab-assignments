#include <stdexcept>
#include "Sequence.h"
#include "DynamicArray.h"

template <typename T>
class ArraySequence: public Sequence<T> {
private:
    DynamicArray<T>* items;
    int count;
    size_t capacity;
public:
    ArraySequence(T* items, int count);
    ArraySequence();
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence();

    virtual T GetFirst() const override;
    virtual T GetLast() const override;
    virtual T Get(int index) const override;
    virtual ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    virtual int GetLength() const override;

    virtual ArraySequence<T>* Append(T item) override;
    virtual ArraySequence<T>* Prepend(T item) override;
    virtual ArraySequence<T>* InsertAt(T item, int index) override;
    virtual Sequence<T>* Concat(Sequence<T>* list) override;
};

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int count){
    if (count < 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    this->capacity = (count > 0) ? (2 * count) : 1;
    this->count = count;    
    items = new DynamicArray<T>(items, count);
}

template <typename T>
ArraySequence<T>::ArraySequence(){
    items = new DynamicArray();
    count = 0;
    capacity = 1;
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& array){
    this->count = array.count;
    this->capacity = array.capacity;
    items = new DynamicArray(*array.items);
}

template <typename T>
ArraySequence<T>::~ArraySequence(){
    delete items;
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    if (count == 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    if (count == 0){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(count - 1);
}

template <typename T>
T ArraySequence<T>::Get(int index) const {
    if (index < 0 || index >= count){
        throw std::invalid_argument("IndexOutOfRange");
    }
    return items->Get(index);
}

template <typename T>
int ArraySequence<T>::GetLength() const {
    return count;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex > count || endIndex < startIndex){
        throw std::invalid_argument("IndexOutOfRange");
    }
    int len = endIndex - startIndex;
    T* data = new T[len];
    for (int i = 0; i < len; ++i){
        data[i] = items->Get(startIndex + i);
    }
    ArraySequence<T>* subSeq = new ArraySequence<T>(data, len);
    delete[] data;
    return subSeq;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Append(T item){
    if (count == capacity){
        capacity = 2 * capacity;
        items->Resize(capacity);
    }
    items->Set(count, item);
    count++;
    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Prepend(T item){
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

template <typename T>
ArraySequence<T>* ArraySequence<T>::InsertAt(T item, int index){
    if (index < 0 || index > count){
        throw std::invalid_argument("IndexOutOfRange");
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

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* array){
    Sequence<T>* newSeq = new ArraySequence<T>(*this);
    for (int i = 0; i < list->GetLength(); i++) {
        newSeq->Append(array->Get(i));
    }
    return newSeq;  
}