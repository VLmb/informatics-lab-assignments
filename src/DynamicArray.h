#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include "CustomErrors.h"

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;
public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& dynamicArray);
    ~DynamicArray();
    
    T& Get(int index) const;
    T* GetData() const;
    void Set(int index, T value);
    size_t GetSize() const;
    void Resize(int newSize);

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
    capacity = (size > 0) ? (2 * size) : 1;
    data = new T[capacity]{};
    for (int i = 0; i < count; ++i) {
        data[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) {
        throw IndexOutOfRange;
    }
    capacity = (size > 0) ? (2 * size) : 1;
    data = new T[capacity]{};
    this->size = size;
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray) {
    data = new T[dynamicArray.capacity]{};
    size = dynamicArray.size;
    capacity = dynamicArray.capacity;
    for (int i = 0; i < dynamicArray.size; ++i) {
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
T* DynamicArray<T>::GetData() const {
    return this->data;
}

template<typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size + 1) {
        std::cout << "this " << index << " " << size << std::endl;
        throw IndexOutOfRange;
    }
    data[index] = value;
    if (index == size) {
        size++;
    }
}

template<typename T>
size_t DynamicArray<T>::GetSize() const {
    return size;
}

template<typename T>
void DynamicArray<T>::Resize(int newSize) { 
    if (newSize < 0) {
        throw NotValidArgument;
    }
    T* newData = new T[newSize]{};
    int copySize = (size < newSize) ? size : newSize;
    for (int i = 0; i < copySize; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newSize;
    size = (newSize < size) ? newSize : size;
}

#endif