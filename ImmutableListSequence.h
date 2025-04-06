#include "ListSequence.h"
#include <stdexcept>

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
private:
    T& operator[](int index) override {
        throw std::logic_error("НИЧЕГО ДРУГОГО НЕ ПРИДУМАЛ");
    }
protected:
    Sequence<T>* Instance() override {
        return new MutableListSequence<T>(*this->items); // Immutable: создаём копию
    }
public:
    ImmutableListSequence(T* data, int count) : ListSequence<T>(data, count) {}
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}
};