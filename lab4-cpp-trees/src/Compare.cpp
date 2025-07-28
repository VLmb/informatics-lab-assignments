#include "Compare.h"

template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

// Генерация случайных данных
std::vector<int> generateRandomData(size_t size) {
    std::vector<int> data(size);
    srand(42);
    for (int i = 0; i < size; ++i) {
        int value = 1 + rand() % (size * 10);
        data[i] = value;
    }
    return data;
}

// Генерация отсортированных данных (возрастающих)
std::vector<int> generateSortedData(size_t size) {
    std::vector<int> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<int>(i);
    }
    return data;
}

std::vector<int> generatePartiallySortedData(size_t size, double sortedFraction) {
    std::vector<int> data = generateRandomData(size);
    size_t sortedCount = static_cast<size_t>(size * sortedFraction);
    std::sort(data.begin(), data.begin() + sortedCount);
    return data;
}

// Тест 1: Вставка случайных данных
void testInsertRandom(size_t size) {
    std::cout << "\n=== Test 1: Insert Random Data (Size: " << size << ") ===\n";
    auto data = generateRandomData(size);

    // AVLTree
    double avlTime = measureTime([&]() {
        AVLTree<int> avl;
        for (const auto& val : data) {
            avl.append(val);
        }
    });
    std::cout << "AVLTree Insert Time: " << avlTime << " ms\n";

    // BST + Balance
    double bstTime = measureTime([&]() {
        BST<int> bst;
        for (const auto& val : data) {
            bst.append(val);
        }
        bst.balance();
    });
    std::cout << "BST + Balance Insert Time: " << bstTime << " ms\n";
}

// Тест 2: Вставка отсортированных данных (деградирующий случай для BST)
void testInsertSorted(size_t size) {
    std::cout << "\n=== Test 2: Insert Sorted Data (Size: " << size << ") ===\n";
    auto data = generateSortedData(size);

    // AVLTree
    double avlTime = measureTime([&]() {
        AVLTree<int> avl;
        for (const auto& val : data) {
            avl.append(val);
        }
    });
    std::cout << "AVLTree Insert Time: " << avlTime << " ms\n";

    // BST + Balance
    double bstTime = measureTime([&]() {
        BST<int> bst;
        for (const auto& val : data) {
            bst.append(val);
        }
        bst.balance();
    });
    std::cout << "BST + Balance Insert Time: " << bstTime << " ms\n";
}

// Тест 3: Вставка частично отсортированных данных
void testInsertPartiallySorted(size_t size, double sortedFraction) {
    std::cout << "\n=== Test 3: Insert Partially Sorted Data (Size: " << size
              << ", Sorted Fraction: " << sortedFraction << ") ===\n";
    auto data = generatePartiallySortedData(size, sortedFraction);

    // AVLTree
    double avlTime = measureTime([&]() {
        AVLTree<int> avl;
        for (const auto& val : data) {
            avl.append(val);
        }
    });
    std::cout << "AVLTree Insert Time: " << avlTime << " ms\n";

    // BST + Balance
    double bstTime = measureTime([&]() {
        BST<int> bst;
        for (const auto& val : data) {
            bst.append(val);
        }
        bst.balance();
    });
    std::cout << "BST + Balance Insert Time: " << bstTime << " ms\n";
}

// Тест 4: Поиск после вставки
void testSearch(size_t size, size_t searchCount) {
    std::cout << "\n=== Test 4: Search After Insert (Size: " << size
              << ", Search Count: " << searchCount << ") ===\n";
    auto data = generateRandomData(size);
    auto searchData = generateRandomData(searchCount);

    AVLTree<int> avl;
    BST<int> bst;
    for (const auto& val : data) {
        avl.append(val);
        bst.append(val);
    }
    // bst.balance();

    // AVLTree Search
    double avlTime = measureTime([&]() {
        for (const auto& val : searchData) {
            avl.search(val);
        }
    });
    std::cout << "AVLTree Search Time: " << avlTime << " ms\n";

    // BST Search
    double bstTime = measureTime([&]() {
        for (const auto& val : searchData) {
            bst.search(val);
        }
    });
    std::cout << "BST Search Time: " << bstTime << " ms\n";
}

// Тест 5: Удаление случайных элементов
void testDelete(size_t size, size_t deleteCount) {
    std::cout << "\n=== Test 5: Delete Random Elements (Size: " << size
              << ", Delete Count: " << deleteCount << ") ===\n";
    auto data = generateRandomData(size);
    auto deleteData = generateRandomData(deleteCount);

    // AVLTree
    AVLTree<int> avl;
    for (const auto& val : data) {
        avl.append(val);
    }
    double avlTime = measureTime([&]() {
        for (const auto& val : deleteData) {
            try {
                avl.erase(val);
            } catch (...) {}
        }
    });
    std::cout << "AVLTree Delete Time: " << avlTime << " ms\n";


    // BST + Balance
    BST<int> bst;
    for (const auto& val : data) {
        bst.append(val);
    }
    bst.balance();
    double bstTime = measureTime([&]() {
        for (const auto& val : deleteData) {
            try {
                bst.erase(val);
            } catch (...) {}
        }
        bst.balance(); // Балансировка после каждого удаления
    });
    std::cout << "BST + Balance Delete Time: " << bstTime << " ms\n";
}

// Тест 6: Комбинированный тест (вставка + поиск + удаление)
void testMixedOperations(size_t size, size_t searchCount, size_t deleteCount) {
    std::cout << "\n=== Test 6: Mixed Operations (Size: " << size
              << ", Search Count: " << searchCount
              << ", Delete Count: " << deleteCount << ") ===\n";
    auto data = generateRandomData(size);
    auto searchData = generateRandomData(searchCount);
    auto deleteData = generateRandomData(deleteCount);

    // AVLTree
    double avlTime = measureTime([&]() {
        AVLTree<int> avl;
        for (const auto& val : data) {
            avl.append(val);
        }
        for (const auto& val : searchData) {
            avl.search(val);
        }
        for (const auto& val : deleteData) {
            try {
                avl.erase(val);
            } catch (...) {}
        }
    });
    std::cout << "AVLTree Mixed Operations Time: " << avlTime << " ms\n";

    // BST + Balance
    double bstTime = measureTime([&]() {
        BST<int> bst;
        for (const auto& val : data) {
            bst.append(val);
        }
        bst.balance();
        for (const auto& val : searchData) {
            bst.search(val);
        }
        for (const auto& val : deleteData) {
            try {
                bst.erase(val);
            } catch (...) {}
        }
        bst.balance();
    });
    std::cout << "BST + Balance Mixed Operations Time: " << bstTime << " ms\n";
}

// Тест 7: Вставка с чередующимися поисками
void testInsertWithSearch(size_t size, size_t searchCount) {
    std::cout << "\n=== Test 7: Insert with Interleaved Search (Size: " << size
              << ", Search Count: " << searchCount << ") ===\n";
    auto data = generateRandomData(size);
    auto searchData = generateRandomData(searchCount);

    // AVLTree
    double avlTime = measureTime([&]() {
        AVLTree<int> avl;
        size_t searchIdx = 0;
        for (size_t i = 0; i < size; i++) {
            avl.append(data[i]);
            if (searchIdx < searchCount) {
                avl.search(searchData[searchIdx]);
                searchIdx++;
            }
        }
    });
    std::cout << "AVLTree Insert+Search Time: " << avlTime << " ms\n";

    // BST без балансировки
    double bstTime = measureTime([&]() {
        BST<int> bst;
        size_t searchIdx = 0;
        for (size_t i = 0; i < size; i++) {
            bst.append(data[i]);
            if (searchIdx < searchCount) {
                bst.search(searchData[searchIdx]);
                searchIdx++;
            }
        }
        // bst.balance(); // Балансировка только в конце
    });
    std::cout << "BST Insert+Search Time (No Balance): " << bstTime << " ms\n";

    // BST с балансировкой
    double bstBalancedTime = measureTime([&]() {
        BST<int> bst;
        size_t searchIdx = 0;
        for (size_t i = 0; i < size; i++) {
            bst.append(data[i]);
            bst.balance();
            if (searchIdx < searchCount) {
                bst.search(searchData[searchIdx]);
                searchIdx++;
            }
        }
    });
    std::cout << "BST Insert+Search+Balance Time: " << bstBalancedTime << " ms\n";
}

// Тест 8: Частые удаления с последующим поиском
void testDeleteWithSearch(size_t size, size_t deleteCount, size_t searchCount) {
    std::cout << "\n=== Test 8: Delete with Search (Size: " << size
              << ", Delete Count: " << deleteCount << ", Search Count: " << searchCount << ") ===\n";
    auto data = generateRandomData(size);
    auto deleteData = generateRandomData(deleteCount);
    auto searchData = generateRandomData(searchCount);

    // AVLTree
    AVLTree<int> avl;
    for (const auto& val : data) {
        avl.append(val);
    }
    double avlTime = measureTime([&]() {
        for (size_t i = 0; i < deleteCount; ++i) {
            try {
                avl.erase(deleteData[i]);
                for (size_t j = 0; j < searchCount / deleteCount; ++j) {
                    avl.search(searchData[i * (searchCount / deleteCount) + j]);
                }
            } catch (...) {}
        }
    });
    std::cout << "AVLTree Delete+Search Time: " << avlTime << " ms\n";

    // BST без балансировки
    BST<int> bst;
    for (const auto& val : data) {
        bst.append(val);
    }
    double bstTime = measureTime([&]() {
        for (size_t i = 0; i < deleteCount; ++i) {
            try {
                bst.erase(deleteData[i]);
                for (size_t j = 0; j < searchCount / deleteCount; ++j) {
                    bst.search(searchData[i * (searchCount / deleteCount) + j]);
                }
            } catch (...) {}
        }
        // bst.balance();
    });
    std::cout << "BST Delete+Search Time (No Balance): " << bstTime << " ms\n";

    // BST с балансировкой
    BST<int> bstBalanced;
    for (const auto& val : data) {
        bstBalanced.append(val);
    }
    double bstBalancedTime = measureTime([&]() {
        for (size_t i = 0; i < deleteCount; ++i) {
            try {
                bstBalanced.erase(deleteData[i]);
                for (size_t j = 0; j < searchCount / deleteCount; ++j) {
                    bstBalanced.search(searchData[i * (searchCount / deleteCount) + j]);
                }
                bstBalanced.balance();
            } catch (...) {}
        }
    });
    std::cout << "BST Delete+Search+Balance Time: " << bstBalancedTime << " ms\n";
}

void runCompareTests() {
    std::cout << "===== Performance Comparison: AVLTree vs BST with Balancing =====\n";

    // Тесты с разными размерами данных
    size_t sizes[] = {1000, 10000, 100000};
    for (size_t size : sizes) {
        testInsertRandom(size);
        testInsertSorted(size);
        testInsertPartiallySorted(size, 0.5);
        testSearch(size, size / 10);
        testDelete(size, size / 10);
        testMixedOperations(size, size / 10, size / 10);
        testInsertWithSearch(size, size / 10);
        testDeleteWithSearch(size, size / 10, size / 10);
        std::cout << "============================================" << std::endl;
    }
}
