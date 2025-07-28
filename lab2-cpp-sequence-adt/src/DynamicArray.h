#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include "CustomErrors.h"

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& dynamicArray);
    ~DynamicArray();
    
    T& Get(int index) const;
    T* GetData() const;
    void Set(int index, T value);
    void Append(T value);
    size_t GetSize() const;
    void Resize(int newSize, const T& el = T());
    void Delete(int index);

};

template<typename T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0) {
        throw NotValidArgument;
    }
    if (!items && count > 0) {
        throw NullPointerPassedAsArgument;
    }
    size = count;
    data = new T[size]{};
    for (int i = 0; i < count; ++i) {
        data[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) {
        throw IndexOutOfRange;
    }
    data = new T[size]{};
    this->size = size;
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray) {
    size = dynamicArray.size;
    data = new T[size]{};
    for (int i = 0; i < size; ++i) {
        data[i] = dynamicArray.data[i];
    }
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template<typename T>
T& DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) {
        throw IndexOutOfRange;
    }
    return data[index];
}

template<typename T>
void DynamicArray<T>::Delete(int index){
    if( index < 0 || index >= size){
        throw IndexOutOfRange;
    }
    T* newData = new T[size-1]{};
    for (int i = 0; i < index; i++){
        newData[i] = data[i];
    }
    for (int i = index; i < size-1; ++i){
        newData[i] = data[i+1];
    }
    delete[] data;
    data = newData;
    size--;
}

template<typename T>
T* DynamicArray<T>::GetData() const {
    return this->data;
}

template<typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index > size) {
        throw IndexOutOfRange;
    }
    if (index == size){
        this->Resize(size + 1);
    }
    data[index] = value;
}

template<typename T>
void DynamicArray<T>::Append(T value){
    this->Resize(size + 1);
    data[size-1] = value;
}


template<typename T>
size_t DynamicArray<T>::GetSize() const {
    return size;
}

template<typename T>
void DynamicArray<T>::Resize(int newSize, const T& el) { 
    if (newSize < 0) {
        throw NotValidArgument;
    }
    T* newData = new T[newSize]{};
    if (newSize < size) {
        for (int i = 0; i < newSize; ++i) {
            newData[i] = data[i];
        }
    }
    else {
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        for (int i = size; i < newSize; ++i){
            newData[i] = el;
        }
    }
    delete[] data;
    data = newData;
    size = newSize;
}

#endif