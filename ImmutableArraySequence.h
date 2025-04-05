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
};