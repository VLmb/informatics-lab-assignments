#include <stdexcept>
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


};

template<typename T>
ListSequence<T>::ListSequence(T* items, int count){
    list = new LinkedList<T>(items, count);
}

template<typename T>
ListSequence<T>::ListSequence(){
    list = new LinkedList<T>();
}

template<typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& otherList){
    list = new LinkedList<T>(otherList);
}

template<typename T>
ListSequence<T>::~ListSequence(){
    delete list;
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
    return list->GetLength();
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