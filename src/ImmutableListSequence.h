#ifndef IMMUTABLE_LIST_H
#define IMMUTABLE_LIST_H

#include "ListSequence.h"
#include <stdexcept>

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    ListSequence<T>* Instance() override {
    MutableListSequence<T>* newSeq = new MutableListSequence<T>(this->items);
    return newSeq;
    }
public:
    ImmutableListSequence(T* data, int count) : ListSequence<T>(data, count) {}
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}

    T& operator[](int index) override {
        throw ChangeImmutableType;
    }
};

#endif