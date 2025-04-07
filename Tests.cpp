#include "Tests.h"

void assert(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << testName << ": [PASSED]\n";
    } else {
        std::cout << testName << ": [FAILED]\n";
    }
}

template<typename T>
bool checkResult(Sequence<T>& seq, T* data, size_t size) {
    try {
        for (int i = 0; i < size; ++i) {
            if (seq[i] != data[i]) { return false; }
        }
    } catch (const std::invalid_argument& e) { // просто стандартная ошибка
        std::cout << "Oops: " << e.what() << "\n";
        return false;
    }
    return true;
}

void testDynamicArray() {
    std::cout << "\n=== Testing DynamicArray ===\n";
    
    // Constr - чек размера и значений
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(data, 3);
    assert(arr.GetSize() == 3, "Constructor with items size");
    assert(arr.Get(0) == 1 && arr.Get(2) == 3, "Constructor with items values");

    // ConstrSize - просто размер
    DynamicArray<int> arr2(5);
    assert(arr2.GetSize() == 5, "Constructor with size");

    // CopyConstr - копирование
    DynamicArray<int> arr3(arr);
    assert(arr3.GetSize() == 3 && arr3.Get(1) == 2, "Copy constructor");

    // Get - выход за пределы
    try {
        arr.Get(5);
        assert(false, "Get with index out of range");
    } catch (const std::invalid_argument&) { // стандартная ошибка
        assert(true, "Get out of range exception");
    }

    // Set - замена значения
    arr.Set(1, 10);
    assert(arr.Get(1) == 10, "Set value");

    // Resize - урезка
    arr.Resize(2);
    assert(arr.GetSize() == 2 && arr.Get(1) == 10, "Resize smaller");
}

void testLinkedList() {
    std::cout << "\n=== Testing LinkedList ===\n";
    
    // Empty - пустой список
    LinkedList<int> list;
    assert(list.GetLength() == 0, "Empty list length");

    // Append/Prepend - добавление в конец и начало
    list.Append(1);
    list.Prepend(0);
    assert(list.GetLength() == 2 && list.GetFirst() == 0 && list.GetLast() == 1, "Append and Prepend");

    // SubList - выдернуть кусок
    list.Append(2);
    LinkedList<int>* subList = list.GetSubList(1, 2);
    assert(subList->GetLength() == 2 && subList->Get(0) == 1, "GetSubList");
    delete subList;

    // GetFirst - пустой список, должен бахнуть
    LinkedList<int> emptyList;
    try {
        emptyList.GetFirst();
        assert(false, "GetFirst on empty list");
    } catch (const std::invalid_argument&) { // стандартная
        assert(true, "GetFirst on empty list exception");
    }

    // Concat - склейка списков
    LinkedList<int> list2;
    list2.Append(3);
    list.Concat(&list2);
    assert(list.GetLength() == 4 && list.Get(3) == 3, "Concat");
}

void testMutableArraySequence() {
    std::cout << "\n=== Testing MutableArraySequence ===\n";
    
    // Constr - создание и доступ
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    assert(checkResult(seq, data, 3), "Constructor and access");

    // Append/Prepend - в конец и начало
    seq.Append(4);
    seq.Prepend(0);
    assert(seq.GetLength() == 5 && seq[0] == 0 && seq[4] == 4, "Append and Prepend");

    // Insert - вставка в середину
    seq.InsertAt(10, 2);
    assert(seq.GetLength() == 6 && seq[2] == 10, "InsertAt");
    // InsertErr - вставка за пределы
    try {
        seq.InsertAt(100, 10);
        assert(false, "InsertAt out of range");
    } catch (const std::invalid_argument&) {
        assert(true, "InsertAt out of range exception");
    }

    // Concat - склейка двух массивов
    MutableArraySequence<int> seq2(data, 3);
    seq.Concat(&seq2);
    int result[9] = {0, 1, 10, 2, 3, 4, 1, 2, 3};
    assert(checkResult(seq, result, 9), "Concat");

    // SubSeq - кусок массива
    Sequence<int>* subSeq = seq.GetSubsequence(1, 3);
    int subData[] = {1, 10, 2};
    assert(checkResult(*subSeq, subData, 3), "GetSubsequence");
    delete subSeq;

    // Map - возведение в квадрат
    auto square = [](const int& x) { return x * x; };
    Sequence<int>* mapped = seq.Map(square);
    assert(mapped->GetLength() == 9 && mapped->Get(0) == 0 && mapped->Get(2) == 100, "Map");
    delete mapped;

    // Reduce - сумма с начальным значением
    auto sum = [](int a, int b) { return a + b; };
    int red_result = seq.Reduce(sum, 10);
    assert(red_result == 29, "Reduce"); // 10 (initial) + 0 + 1 + 10 + 2 + 3 + 4 + 1 + 2 + 3

    // Iter - пробежка по итератору
    int iter_sum = 0;
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        iter_sum += *it;
    }
    assert(iter_sum == 19, "Iterator sum"); // 0 + 1 + 10 + 2 + 3 + 4 + 1 + 2 + 3
}

void testImmutableArraySequence() {
    std::cout << "\n=== Testing ImmutableArraySequence ===\n";
    
    // Append - добавление с неизменностью
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);
    Sequence<int>* newSeq = seq.Append(4);
    assert(seq.GetLength() == 3 && newSeq->GetLength() == 4, "Immutable Append");
    delete newSeq;

    // Op[] - попытка записи в immutable
    try {
        seq[0] = 5;
        assert(false, "Immutable operator[] write");
    } catch (const std::logic_error&) { // оставим как было, это не IndexOutOfRange
        assert(true, "Immutable operator[] write exception");
    }
}

void testMutableListSequence() {
    std::cout << "\n=== Testing MutableListSequence ===\n";
    
    // Constr - создание и доступ
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);
    assert(checkResult(seq, data, 3), "Constructor and access");

    // Append/Prepend - в конец и начало
    seq.Append(4);
    seq.Prepend(0);
    assert(seq.GetLength() == 5 && seq[0] == 0 && seq[4] == 4, "Append and Prepend");

    // Insert - вставка в середину
    seq.InsertAt(10, 2);
    assert(seq.GetLength() == 6 && seq[2] == 10, "InsertAt");
    // InsertErr - вставка за пределы
    try {
        seq.InsertAt(100, 10);
        assert(false, "InsertAt out of range");
    } catch (const std::invalid_argument&) {
        assert(true, "InsertAt out of range exception");
    }

    // Concat - склейка списков
    MutableListSequence<int> seq2(data, 3);
    seq.Concat(&seq2);
    int result[9] = {0, 1, 10, 2, 3, 4, 1, 2, 3};
    assert(checkResult(seq, result, 9), "Concat");

    // SubSeq - кусок списка
    Sequence<int>* subSeq = seq.GetSubsequence(1, 3);
    int subData[] = {1, 10, 2};
    assert(checkResult(*subSeq, subData, 3), "GetSubsequence");
    delete subSeq;

    // Map - возведение в квадрат
    auto square = [](const int& x) { return x * x; };
    Sequence<int>* mapped = seq.Map(square);
    assert(mapped->GetLength() == 9 && mapped->Get(0) == 0 && mapped->Get(2) == 100, "Map");
    delete mapped;

    // Reduce - сумма с начальным значением
    auto sum = [](int a, int b) { return a + b; };
    int red_result = seq.Reduce(sum, 10);
    assert(red_result == 29, "Reduce"); // 10 (initial) + 0 + 1 + 10 + 2 + 3 + 4 + 1 + 2 + 3

    // Iter - пробежка по итератору
    int iter_sum = 0;
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        iter_sum += *it;
    }
    assert(iter_sum == 19, "Iterator sum"); // 0 + 1 + 10 + 2 + 3 + 4 + 1 + 2 + 3
}

void testImmutableListSequence() {
    std::cout << "\n=== Testing ImmutableListSequence ===\n";
    
    // Prepend - добавление в начало с неизменностью
    int data[] = {1, 2};
    ImmutableListSequence<int> seq(data, 2);
    Sequence<int>* newSeq = seq.Prepend(0);
    assert(seq.GetLength() == 2 && newSeq->GetLength() == 3 && newSeq->Get(0) == 0, "Immutable Prepend");
    delete newSeq;
}

int runTests() {
    testDynamicArray();
    testLinkedList();
    testMutableArraySequence();
    testImmutableArraySequence();
    testMutableListSequence();
    testImmutableListSequence();
    return 0;
}