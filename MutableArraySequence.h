#include "ArraySequence.h"

template<typename T>
class MutableArraySequence: public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this;
    }
 public:
    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence() : ArraySequence<T>() {}
    
};