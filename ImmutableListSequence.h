#include "ListSequence.h"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return new MutableListSequence<T>(*this->items); // Immutable: создаём копию
    }
public:
    ImmutableListSequence(T* data, int count) : ListSequence<T>(data, count) {}
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}
};