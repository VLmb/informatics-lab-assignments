#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include "CustomErrors.h"
#include <functional>
#include "Iterator.h"

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
    void sort();

    class DynamicArrayIterator: public Iterator<T> {
    private:
        T* ptr;
        friend class DynamicArray;

    public:
        DynamicArrayIterator(T* ptr = nullptr) : ptr(ptr) {}

        T& operator*() override { return *ptr; }

        Iterator<T>& operator++() override { ++ptr; return *this; }
        Iterator<T>& operator--() override { --ptr; return *this; }

        bool operator==(const DynamicArrayIterator& other) const { return ptr == other.ptr; }
        bool operator!=(const DynamicArrayIterator& other) const { return ptr != other.ptr; }
    };

    DynamicArrayIterator erase(DynamicArrayIterator it);

    DynamicArrayIterator begin() { return DynamicArrayIterator(data); }
    DynamicArrayIterator end() { return DynamicArrayIterator(data + size); }
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
    if (size == 0){
        throw EmptyContainer;
    }
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

template<typename T>
typename DynamicArray<T>::DynamicArrayIterator DynamicArray<T>::erase(DynamicArrayIterator it) {
    if (it == end()) {
        throw NotValidArgument;
    }

    T* pos = it.ptr;

    size_t index = pos - data;

    if (index >= size) {
        throw IndexOutOfRange;
    }

    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }

    size--;

    if (index == size) {
        return DynamicArrayIterator(data + size);
    } else {
        return DynamicArrayIterator(data + index);
    }
}

template<typename T>
void DynamicArray<T>::sort() {
    auto swap = [](T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    };

    auto medianOfThree = [&](int low, int high) -> T {
        int mid = (low + high) / 2;

        if (data[low] > data[mid]) swap(data[low], data[mid]);
        if (data[low] > data[high]) swap(data[low], data[high]);
        if (data[mid] > data[high]) swap(data[mid], data[high]);

        swap(data[mid], data[high]);
        return data[high];
    };

    std::function<int(int, int)> partition = [&](int low, int high) -> int {
        T pivot = medianOfThree(low, high);
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (data[j] <= pivot) {
                ++i;
                swap(data[i], data[j]);
            }
        }

        swap(data[i + 1], data[high]);
        return i + 1;
    };

    std::function<void(int, int)> quickSort = [&](int low, int high) {
        if (low < high) {
            int pi = partition(low, high);
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
    };

    quickSort(0, size - 1);
}


#endif