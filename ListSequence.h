#include <stdexcept>
#include "Sequence.h"
#include "LinkedList.h"

template<typename T>
class ListSequence: public Sequence<T> {
private:
    LinkedList<T>* list;
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

    virtual ListSequence<T>* Append(T item) override;
    virtual ListSequence<T>* Prepend(T item) override;
    virtual ListSequence<T>* InsertAt(T item, int index) override;
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
ListSequence<T>* ListSequence<T>::Append(T item) {
    list->Append(item);
    return this;
}

template<typename T>
ListSequence<T>* ListSequence<T>::Prepend(T item) {
    list->Prepend(item);
    return this;
}

template<typename T>
ListSequence<T>* ListSequence<T>::InsertAt(T item, int index) {
    list->InsertAt(item, index);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* otherList) {
    for (int i = 0; i < otherList->GetLength(); i++) {
        this->Append(otherList->Get(i));
    }
    return this;
}

