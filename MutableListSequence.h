#include "ListSequence.h"

template <typename T>
class MutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this; // Mutable: изменяем текущий объект
    }
public:
    MutableListSequence(T* data, int count) : ListSequence<T>(data, count) {}
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}
};