#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include "CustomErrors.h"
#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "Interface.h"

template<typename T>
Wrapper<T>::Wrapper(const std::string& structure_, const std::string& type_, bool immutable_)
    : structure(structure_), type_key(type_), immutable(immutable_) {
    MutableArraySequence<T> values = getArrayInput<T>("Enter initial elements (space-separated, or press Enter for empty): ");
    if (structure == "array") {
        seq = immutable ? std::make_shared<ImmutableArraySequence<T>>(values.GetData(), values.GetLength())
                        : std::make_shared<MutableArraySequence<T>>(values.GetData(), values.GetLength());
    } else {
        seq = immutable ? std::make_shared<ImmutableListSequence<T>>(values.GetData(), values.GetLength())
                        : std::make_shared<MutableListSequence<T>>(values.GetData(), values.GetLength());
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
        std::shared_ptr<Wrapper<T>> newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
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
        std::shared_ptr<Wrapper<T>> newWrapper = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
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
        throw Errors::ConcatTypeMismatchError();
    }
    auto newSeq = seq->Concat(other_casted->seq.get());
    result = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
    result->seq = std::shared_ptr<Sequence<T>>(newSeq);
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
    auto square = [](const T& x) {
        if constexpr (std::is_same_v<T, std::string>) {
            throw Errors::InvalidOperation();
        } else {
            return x * x;
        }
    };
    auto newSeq = seq->Map(square);
    auto wrapped = std::make_shared<Wrapper<T>>(structure, type_key, immutable);
    wrapped->seq = std::shared_ptr<Sequence<T>>(newSeq);
    sequences.Append(wrapped);
}

template<typename T>
void Wrapper<T>::doReduce() const {
    auto sum = [](T a, T b) {
        return a + b;
    };
    T result = seq->Reduce(sum, T{});
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

template class Wrapper<int>;
template class Wrapper<float>;
template class Wrapper<std::string>;