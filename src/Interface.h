#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <iostream>
#include "CustomErrors.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"

class InterfaceWrapper {
public:
    virtual ~InterfaceWrapper() {}
    virtual void doAppend(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual void doPrepend(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual void doInsertAt(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual void doGetSub() = 0;
    virtual void doDelete(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual void doMap(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual void doReduce() = 0;
    virtual void doPrint() = 0;
    virtual void doConcat(MutableArraySequence<InterfaceWrapper*>& sequences) = 0;
    virtual std::string TypeKey() = 0;
    virtual std::string Structure() = 0;
    virtual int GetLength() = 0;
    virtual std::string GetMutable() = 0;
};

void clearInput();
template<typename T> T getTypedInput();
int getIntInput();
std::string getStringInput();
template<typename T> MutableArraySequence<T> getArrayInput(std::string type);
void ShowMenu();
InterfaceWrapper* ChooseSequence(std::string& name);
void CreateSequence(MutableArraySequence<InterfaceWrapper*>& seqs);
int PickSequence(MutableArraySequence<InterfaceWrapper*>& seqs);
void runUI();

template<typename T>
class Wrapper : public InterfaceWrapper {
public:
    Sequence<T>* seq;
    std::string structure;
    std::string type_key;
    bool immutable;

    Wrapper(std::string structure_, std::string type_, bool immutable_, T* data = NULL, int count = 0);
    ~Wrapper();

    void doAppend(MutableArraySequence<InterfaceWrapper*>& sequences);
    void doPrepend(MutableArraySequence<InterfaceWrapper*>& sequences);
    void doInsertAt(MutableArraySequence<InterfaceWrapper*>& sequences);
    void doGetSub();
    void doDelete(MutableArraySequence<InterfaceWrapper*>& sequences);
    void doMap(MutableArraySequence<InterfaceWrapper*>& sequences);
    void doReduce();
    void doPrint();
    void doConcat(MutableArraySequence<InterfaceWrapper*>& sequences);
    std::string TypeKey();
    std::string Structure();
    int GetLength();
    std::string GetMutable();

};

template<typename T>
Wrapper<T>::Wrapper(std::string structure_, std::string type_, bool immutable_, T* data, int count)
    : structure(structure_), type_key(type_), immutable(immutable_) {
    if (structure == "array") {
        if (immutable)
            seq = new ImmutableArraySequence<T>(data, count);
        else
            seq = new MutableArraySequence<T>(data, count);
    } else {
        if (immutable)
            seq = new ImmutableListSequence<T>(data, count);
        else
            seq = new MutableListSequence<T>(data, count);
    }
}

template<typename T>
Wrapper<T>::~Wrapper() {
    delete seq;
}

template<typename T>
void Wrapper<T>::doAppend(MutableArraySequence<InterfaceWrapper*>& sequences) {
    std::cout << "\nEnter a value to append (type: " << type_key << "): ";
    T value = getTypedInput<T>();
    Sequence<T>* newSeq = seq->Append(value);

    if (newSeq != seq) {
        Wrapper<T>* newWrapper = new Wrapper<T>(structure, type_key, immutable);
        newWrapper->seq = newSeq;
        sequences.Append(newWrapper);
    }
}

template<typename T>
void Wrapper<T>::doPrepend(MutableArraySequence<InterfaceWrapper*>& sequences) {
    std::cout << "\nEnter a value to prepend (type: " << type_key << "): ";
    T value = getTypedInput<T>();
    Sequence<T>* newSeq = seq->Prepend(value);
    if (newSeq != seq) {
        Wrapper<T>* newWrapper = new Wrapper<T>(structure, type_key, immutable);
        newWrapper->seq = newSeq;
        sequences.Append(newWrapper);
    }
}

template<typename T>
void Wrapper<T>::doInsertAt(MutableArraySequence<InterfaceWrapper*>& sequences) {
    std::cout << "\nEnter the index to insert at (0 to " << seq->GetLength() << "): ";
    int index = getIntInput();
    if (index < 0 || index > seq->GetLength()) {
        std::cout << "Error: Index must be between 0 and " << seq->GetLength() << "\n";
        return;
    }
    std::cout << "Enter a value to insert (type: " << type_key << "): ";
    T value = getTypedInput<T>();
    Sequence<T>* newSeq = seq->InsertAt(value, index);
    if (newSeq != seq) {
        Wrapper<T>* newWrapper = new Wrapper<T>(structure, type_key, immutable);
        newWrapper->seq = newSeq;
        sequences.Append(newWrapper);
    }
}

template<typename T>
void Wrapper<T>::doGetSub() {
    std::cout << "Enter the start index (0 to " << seq->GetLength() - 1 << "): ";
    int start = getIntInput();
    if (start < 0 || start >= seq->GetLength()) {
        throw IndexOutOfRange;
    }
    std::cout << "Enter the end index (" << start << " to " << seq->GetLength() - 1 << "): ";
    int end = getIntInput();
    if (end < start || end >= seq->GetLength()) {
        throw IndexOutOfRange;
    }
    Sequence<T>* subSeq = seq->GetSubsequence(start, end);
    std::cout << "Subsequence:  [";
    for (int i = 0; i < subSeq->GetLength(); ++i) {
        std::cout << subSeq->Get(i);
        if (i < subSeq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    delete subSeq;
}

template<typename T>
void Wrapper<T>::doDelete(MutableArraySequence<InterfaceWrapper*>& sequences) {
    if (seq->GetLength() == 0){
        std::cout << "Empty sequence, unable to delete any element\n";
        return;
    }
    std::cout << "Enter the index (0 to " << seq->GetLength() - 1 << "): ";
    int index = getIntInput();
    if (index < 0 || index >= seq->GetLength()) {
        throw IndexOutOfRange;
    }
    Sequence<T>* newSeq = seq->Delete(index);
    if (newSeq != seq) {
        Wrapper<T>* newWrapper = new Wrapper<T>(structure, type_key, immutable);
        newWrapper->seq = newSeq;
        sequences.Append(newWrapper);
    }
}

template<typename T>
void Wrapper<T>::doMap(MutableArraySequence<InterfaceWrapper*>& sequences) {
    Sequence<T>* newSeq = seq->Map([](const T& x) { return x + x; });
    Wrapper<T>* wrapped = new Wrapper<T>(structure, type_key, immutable);
    wrapped->seq = newSeq;
    sequences.Append(wrapped);
}

template<typename T>
void Wrapper<T>::doReduce() {
    if (type_key == "string") {
        throw TypeMismatchError;
    }
    T result = 0;
    for (int i = 0; i < seq->GetLength(); ++i) {
        result = result + seq->Get(i);
    }
    std::cout << result << "\n";
}

template<typename T>
void Wrapper<T>::doPrint() {
    std::cout << std::endl;
    std::cout << "Sequence:  ";
    std::cout << "[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

template<typename T>
void Wrapper<T>::doConcat(MutableArraySequence<InterfaceWrapper*>& sequences) {
    int index = PickSequence(sequences);
    if (index == -1) {
        std::cout << "\nNo sequence selected\n";
        return;
    }
    InterfaceWrapper* other = sequences.Get(index);
    if (structure != other->Structure() || type_key != other->TypeKey()) {
        throw TypeMismatchError;
    }
    Wrapper<T>* other_casted = (Wrapper<T>*)other;
    Sequence<T>* newSeq = seq->Concat(other_casted->seq);
    bool flag;
    if (newSeq != seq) {
        if (immutable = true) { flag = false; }
        else { flag = false; }
        Wrapper<T>* newWrapper = new Wrapper<T>(structure, type_key, flag);
        newWrapper->seq = newSeq;
        sequences.Append(newWrapper);
    }
}

template<typename T>
std::string Wrapper<T>::TypeKey() {
    return type_key;
}

template<typename T>
std::string Wrapper<T>::Structure() {
    return structure;
}

template<typename T>
int Wrapper<T>::GetLength() {
    return seq->GetLength();
}

template<typename T>
std::string Wrapper<T>::GetMutable() {
    if (immutable) return "immutable sequence";
    else { return "mutable sequence"; }
}

#endif