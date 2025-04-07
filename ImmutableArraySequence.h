#include <stdexcept>

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        MutableArraySequence<T>* newArray = new MutableArraySequence<T>(this->items->GetData(), this->count);
        return newArray;
    }
public:
    ImmutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence() : ArraySequence<T>() {}

    T& operator[](int index) override {
        throw std::logic_error("An attempt to change an immutable object");
    }
};