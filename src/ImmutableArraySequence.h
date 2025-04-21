#ifndef IMMUTABLE_ARRAY_H
#define IMMUTABLE_ARRAY_H

#include <stdexcept>
#include "CustomErrors.h"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* Instance() override {
        MutableArraySequence<T>* newArray = new MutableArraySequence<T>(*this);
        return newArray;
    }
public:
    ImmutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence() : ArraySequence<T>() {}

    T& operator[](int index) override {
        throw ChangeImmutableType;
    }
};

#endif