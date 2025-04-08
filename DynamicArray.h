#include <stdexcept>

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
    
    T Get(int index) const;
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
    data = new T[count]{};
    size = count;
    for (int i = 0; i < count; ++i) {
        data[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) {
        throw IndexOutOfRange;
    }
    data = new T[size];
    this->size = size;
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray) {
    data = new T[dynamicArray.size];
    size = dynamicArray.size;
    for (int i = 0; i < dynamicArray.size; ++i) {
        data[i] = dynamicArray.data[i];
    }
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template<typename T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) {
        throw IndexOutOfRange;
    }
    return data[index];
}

template<typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) {
        throw IndexOutOfRange;
    }
    data[index] = value;
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
    size = newSize;
}
