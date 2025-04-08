#include <stdexcept>
#include <functional>
#include "Sequence.h"
#include "LinkedList.h"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* items;

    virtual Sequence<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& item) {
        items->Append(item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& item) {
        items->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& item, int index) {
        items->InsertAt(item, index);
        return this;
    }

    Sequence<T>* ConcatInternal(Sequence<T>* list) {
        for (int i = 0; i < list->GetLength(); i++) {
            items->Append(list->Get(i));
        }
        return this;
    }

public: 
    ListSequence(T* items, int count);
    ListSequence();
    ListSequence(const LinkedList<T>& otherList);
    virtual ~ListSequence();

    virtual T GetFirst() const override;
    virtual T GetLast() const override;
    virtual T Get(int index) const override;
    virtual ListSequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    virtual int GetLength() const override;

    virtual Sequence<T>* Append(const T& item) override;
    virtual Sequence<T>* Prepend(const T& item) override;
    virtual Sequence<T>* InsertAt(const T& item, int index) override;
    virtual Sequence<T>* Concat(Sequence<T>* list) override;

    virtual const T& operator[](int index) const override;
    virtual T& operator[](int index) override;
    virtual bool operator==(const Sequence<T>& otherList) override;
    virtual bool operator!=(const Sequence<T>& otherList) override;

    virtual Sequence<T>* Map(std::function<T(const T&)> f) const override;
    virtual T Reduce(std::function<T(const T&, const T&)> f, T initial) const override;
    virtual Sequence<T>* Find(std::function<bool(const T&)> f) const override;
    template<typename K>
    virtual Sequence<std::tuple<T, K>>* Zip(ListSequence<K>& otherList) const override;
    template<typename K, typename P>
    virtual std::tuple<Sequence<K>*, Sequence<P>*> Unzip() const override;

    class Iterator {
    private:
        const ListSequence<T>* data;
        size_t index;
    public:
        Iterator(ListSequence<T>* data, size_t& index) : data(data), index(index) {}

        T& Get(){return data->Get(i); }
        void Next() { index++; }

        T& operator*(){ return data->Get(index); }
        Iterator& operator++() { index++; return *this; }
        bool operator!=(const Iterator& other ){ return index != other.index; }
        bool operator==(const Iterator& other) const { return index == other.index; }
    };

    Iterator begin() const { return Iterator(this, 0); };
    Iterator end() const { return Iterator(this, count); };
};

template <typename T>
const T& ListSequence<T>::operator[](int index) const {
    if (index < 0 || index >= count){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
T& ListSequence<T>::operator[](int index){
    if (index < 0 || index >= count){
        throw IndexOutOfRange;
    }
    return items->Get(index);
}

template <typename T>
bool ListSequence<T>::operator==(const Sequence<T>& otherList){
    return items->Equal(otherList);
}

template <typename T>
bool ListSequence<T>::operator!=(const Sequence<T>& otherList){
    return !(items->Equal(otherList));
}

template<typename T>
ListSequence<T>::ListSequence(T* items, int count){
    items = new LinkedList<T>(items, count);
}

template<typename T>
ListSequence<T>::ListSequence(){
    items = new LinkedList<T>();
}

template<typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& otherList){
    items = new LinkedList<T>(otherList);
}

template<typename T>
ListSequence<T>::~ListSequence(){
    delete items;
}

template<typename T>
T ListSequence<T>::GetFirst() const{
    return list->GetFirst();
}

template<typename T>
T ListSequence<T>::GetLast() const {
    return list->GetLast();
}

template<typename T>
T ListSequence<T>::Get(int index) const {
    return list->Get(index);
}

template<typename T>
ListSequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    LinkedList<T>* newList = list->GetSubList(startIndex, endIndex);
    ListSequence<T>* subSeq = new ListSequence<T>(*newList);
    delete newList;
    return subSeq;
}

template<typename T>
int ListSequence<T>::GetLength() const {
    return items->GetLength();
}

template<typename T>
Sequence<T>* ListSequence<T>::Append(const T& item) {
    return Instance()->AppendInternal(item);
}

template<typename T>
Sequence<T>* ListSequence<T>::Prepend(const T& item) {
    return Instance()->PrependInternal(item);
}

template<typename T>
Sequence<T>* ListSequence<T>::InsertAt(const T& item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* otherList) {
    return Instance()->ConcatInternal(list);
}

template<typename T>
Sequence<T>* ListSequence<T>::Map(std::function<T(const T&)> f) const {
    MutableListSequence<T>* result = new MutableListSequence<T>();
    for (int i = 0; i < this->GetLength(); ++i){
        result->Append(f(this->Get(i)));
    }
    return result;
}

template<typename T>
T ListSequence<T>::Reduce(std::function<T(const T&, const T&)> f, T initial) const {
    T result = initial;
    for (int i = 0; i < this->GetLength(); ++i){
        result = f(result, this->Get(i));
    }
    return result;
}

template<typename T>
Sequence<T>* ListSequence<T>::Find(std::function<bool(const T&)> f) const {
    MutableListSequence<T>* result = new MutableListSequence<T>();
    for (int i = 0; i < this->GetLength; ++i){
        T cur = this->Get(i);
        if(f(cur)){
            result->Append(cur);
        }
    }
    return result;
}

template<typename T>
template<typename K>
Sequence<std::tuple<T, K>>* ListSequence<T>::Zip(ListSequence<K>& otherList) const {
    MutableListSequence<std::tuple<T, K>>* result = new MutableListSequence<std::tuple<T, K>>();
    int size = (count < otherList->GetLength()) ? count : otherList->GetLength();
    for (int i = 0; i < size; ++i){
        result->Append(std::make_tuple(this->Get(i), otherList->Get(i)));
    }
    return result;
}

template<typename T>
template<typename K, typename P>
std::tuple<Sequence<K>*, Sequence<P>*> ListSequence<T>::Unzip() const {
    //НЕОБХОДИМО ПРОВЕРИТЬ, ЯВЛЯЕТСЯ ЛИ ПЕРЕДАННОЕ КОРТЕЖЕМ

    MutableListSequence<U>* firstSeq = new MutableListSequence<K>();
    MutableListSequence<V>* secondSeq = new MutableListSequence<P>();

    for (int i = 0; i < count; ++i) {
        T tuple = items->Get(i);
        firstSeq->Append(std::get<0>(tuple));
        secondSeq->Append(std::get<1>(tuple));
    }

    return std::make_tuple(firstSeq, secondSeq);
}
