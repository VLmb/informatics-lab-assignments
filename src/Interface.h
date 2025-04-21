#ifndef INTERFACE_H
#define INTERFACE_H

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include "CustomErrors.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"

class InterfaceWrapper {
public:
    virtual ~InterfaceWrapper() = default;
    virtual void doAppend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) = 0;
    virtual void doPrepend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) = 0;
    virtual void doInsertAt(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) = 0;
    virtual void doConcat(const std::shared_ptr<InterfaceWrapper>& other, std::shared_ptr<InterfaceWrapper>& result) = 0;
    virtual void doGetSub() const = 0;
    virtual void doMap(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences, const std::string& name) = 0;
    virtual void doReduce() const = 0;
    virtual void doPrint() const = 0;
    virtual std::string TypeKey() const = 0;
    virtual std::string Structure() const = 0;
    virtual int GetLength() const = 0;
};

void clearInput();
template<typename T> T getTypedInput(const std::string& prompt = "Enter value: ");
int getIntInput(const std::string& prompt = "");
std::string getStringInput(const std::string& prompt);
template<typename T> MutableArraySequence<T> getArrayInput(const std::string& prompt = "");
void ShowMenu();
std::shared_ptr<InterfaceWrapper> ChooseSequence(std::string& name);
void CreateSequence(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& seqs);
int PickSequence(const MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& seqs);
void runUI();

template<typename T>
class Wrapper : public InterfaceWrapper {
public:
    std::shared_ptr<Sequence<T>> seq;
    std::string structure;
    std::string type_key;
    bool immutable;

    Wrapper(const std::string& structure_, const std::string& type_, bool immutable_, T* data = nullptr, int count = 0);

    void doAppend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) override;
    void doPrepend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) override;
    void doInsertAt(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) override;
    void doConcat(const std::shared_ptr<InterfaceWrapper>& other, std::shared_ptr<InterfaceWrapper>& result) override;
    void doGetSub() const override;
    void doMap(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences, const std::string& name) override;
    void doReduce() const override;
    void doPrint() const override;
    std::string TypeKey() const override;
    std::string Structure() const override;
    int GetLength() const override;
};

template<typename T>
Wrapper<T>::Wrapper(const std::string& structure_, const std::string& type_, bool immutable_, T* data, int count)
    : structure(structure_), type_key(type_), immutable(immutable_) {
    if (structure == "array") {
        if (immutable)
            seq = std::make_shared<ImmutableArraySequence<T>>(data, count);
        else
            seq = std::make_shared<MutableArraySequence<T>>(data, count);
    } else {
        if (immutable)
            seq = std::make_shared<ImmutableListSequence<T>>(data, count);
        else
            seq = std::make_shared<MutableListSequence<T>>(data, count);
    }
}

template<typename T>
void Wrapper<T>::doAppend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) {
    T value = getTypedInput<T>("Enter value to append: ");
    Sequence<T>* newSeq = seq->Append(value);
    if (newSeq != seq.get()) {
        auto newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
        newWrapper->seq = std::shared_ptr<Sequence<T>>(newSeq);
        std::string name = getStringInput("Enter name for new sequence (or press Enter for default): ");
        if (name.empty() || name == "Unnamed") {
            name = "New_" + type_key + "_" + std::to_string(sequences.GetLength());
        }
        sequences.Append(newWrapper);
        std::cout << "Created new sequence '" << name << "' at index " << sequences.GetLength() - 1 << "\n";
    }
}

template<typename T>
void Wrapper<T>::doPrepend(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) {
    T value = getTypedInput<T>("Enter value to prepend: ");
    Sequence<T>* newSeq = seq->Prepend(value);
    if (newSeq != seq.get()) {
        auto newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
        newWrapper->seq = std::shared_ptr<Sequence<T>>(newSeq);
        std::string name = getStringInput("Enter name for new sequence (or press Enter for default): ");
        if (name.empty() || name == "Unnamed") {
            name = "New_" + type_key + "_" + std::to_string(sequences.GetLength());
        }
        sequences.Append(newWrapper);
        std::cout << "Created new sequence '" << name << "' at index " << sequences.GetLength() - 1 << "\n";
    }
}

template<typename T>
void Wrapper<T>::doInsertAt(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences) {
    int index = getIntInput("Enter index: ");
    T value = getTypedInput<T>("Enter value: ");
    Sequence<T>* newSeq = seq->InsertAt(value, index);
    if (newSeq != seq.get()) {
        auto newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
        newWrapper->seq = std::shared_ptr<Sequence<T>>(newSeq);
        std::string name = getStringInput("Enter name for new sequence (or press Enter for default): ");
        if (name.empty() || name == "Unnamed") {
            name = "New_" + type_key + "_" + std::to_string(sequences.GetLength());
        }
        sequences.Append(newWrapper);
        std::cout << "Created new sequence '" << name << "' at index " << sequences.GetLength() - 1 << "\n";
    }
}

template<typename T>
void Wrapper<T>::doConcat(const std::shared_ptr<InterfaceWrapper>& other, std::shared_ptr<InterfaceWrapper>& result) {
    auto other_casted = std::dynamic_pointer_cast<Wrapper<T>>(other);
    if (!other_casted || other_casted->structure != structure || other_casted->type_key != type_key) {
        throw NotValidArgument;
    }
    auto newSeq = seq->Concat(other_casted->seq.get());
    auto newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
    newWrapper->seq = std::shared_ptr<Sequence<T>>(newSeq);
    result = newWrapper;
}

template<typename T>
void Wrapper<T>::doGetSub() const {
    int start = getIntInput("Enter start index: ");
    int end = getIntInput("Enter end index: ");
    auto subSeq = seq->GetSubsequence(start, end);
    std::cout << "Subsequence: [";
    for (int i = 0; i < subSeq->GetLength(); ++i) {
        if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "\"" << subSeq->Get(i) << "\"";
        } else {
            std::cout << subSeq->Get(i);
        }
        if (i < subSeq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    delete subSeq;
}

template<typename T>
void Wrapper<T>::doMap(MutableArraySequence<std::shared_ptr<InterfaceWrapper>>& sequences, const std::string& name) {
    auto sum = [](const T& x) { return x + x; };
    auto newSeq = seq->Map(sum);
    auto wrapped = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
    wrapped->seq = std::shared_ptr<Sequence<T>>(newSeq);
    sequences.Append(wrapped);
}

template<typename T>
void Wrapper<T>::doReduce() const {
    auto sum = [](T a, T b) {
        return a + b;
    };
    T result = seq->Reduce(sum, 0);
    std::cout << "Result: " << result << "\n";
}

template<typename T>
void Wrapper<T>::doPrint() const {
    std::cout << "Sequence: [";
    for (int i = 0; i < seq->GetLength(); ++i) {
        if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "\"" << seq->Get(i) << "\"";
        } else {
            std::cout << seq->Get(i);
        }
        if (i < seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

template<typename T>
std::string Wrapper<T>::TypeKey() const {
    return type_key;
}

template<typename T>
std::string Wrapper<T>::Structure() const {
    return structure;
}

template<typename T>
int Wrapper<T>::GetLength() const {
    return seq->GetLength();
}

#endif