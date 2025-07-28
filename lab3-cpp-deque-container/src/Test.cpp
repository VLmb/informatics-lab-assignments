#include "Test.h"

void assert_custom(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << testName << " - [PASSED]\n";
    } else {
        std::cout << testName << " - [FAILED]\n";
    }
}

template<typename T>
bool cmp(typename DequeList<T>::DequeListIterator it, T* data, size_t size){
    for (size_t i = 0; i < size; ++i, ++it) {
        if (*it != data[i]){
            return false;
        }
    }
    return true;
}

template<typename T>
bool cmp(typename DequeArray<T>::DequeArrayIterator it, T* data, size_t size){
    for (size_t i = 0; i < size; ++i, ++it) {
        if (*it != data[i]){
            return false;
        }
    }
    return true;
}

template<typename T>
bool checkResult(DequeList<T>& deq, T* data, size_t size) {
    if (deq.length() != size) return false;
    try {
        if (!cmp(deq.begin(), data, size)) return false;
    } catch (Errors error) {
        std::cout << "Error in checkResult: " << error << "\n";
        return false;
    }
    return true;
}

template<typename T>
bool checkResult(DequeArray<T>& deq, T* data, size_t size) {
    if (deq.length() != size) return false;
    try {
        if (!cmp(deq.begin(), data, size)) return false;
    } catch (Errors error) {
        std::cout << "Error in checkResult: " << error << "\n";
        return false;
    }
    return true;
}

void testDequeList() {
    std::cout << "\n==== Testing Deque ====\n";

    // Test 1: Дэфолт конструктор
    std::cout << "\n### Default constructor ###\n";
    DequeList<int> deq1;
    assert_custom(deq1.empty(), "Empty");
    assert_custom(deq1.length() == 0, "Length");

    // Test 2: Конструктор
    std::cout << "\n### Constructor ###\n";
    int data[] = {1, 2, 3};
    DequeList<int> deq2(data, 3);
    assert_custom(deq2.length() == 3, "Constructor with items: length");
    assert_custom(checkResult(deq2, data, 3), "Constructor with items: values");

    // Test 3: Push_front и push_back 
    std::cout << "\n### Push_front and push_back ###\n";
    DequeList<int> deq3;
    deq3.push_front(1);
    deq3.push_back(2);
    deq3.push_front(0);
    int push_data[] = {0, 1, 2};
    assert_custom(checkResult(deq3, push_data, 3), "Push_front and push_back");

    // Test 4: Front и back
    std::cout << "\n### Font and back ###\n";
    assert_custom(deq3.front() == 0, "Front value");
    assert_custom(deq3.back() == 2, "Back value");

    // Test 5: Front и back на пусой дэке
    DequeList<int> empty_deq;
    try {
        empty_deq.front();
        assert_custom(false, "Front on empty deque");
    } catch (Errors error) {
        assert_custom(error == EmptyContainer, "Front on empty deque exception");
    }
    try {
        empty_deq.back();
        assert_custom(false, "Back on empty deque");
    } catch (Errors error) {
        assert_custom(error == EmptyContainer, "Back on empty deque exception");
    }

    // Test 6: Pop_front и pop_back
    std::cout << "\n### Pop_front and pop_back ###\n";
    DequeList<int> deq4(data, 3);
    deq4.pop_front();
    deq4.pop_back();
    int pop_data[] = {2};
    assert_custom(checkResult(deq4, pop_data, 1), "Pop_front and pop_back");

    // Test 7: Pop на пустой
    DequeList<int> deq5;
    try {
        deq5.pop_front();
        assert_custom(false, "Pop_front on empty deque");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "Pop_front on empty deque exception");
    }
    try {
        deq5.pop_back();
        assert_custom(false, "Pop_back on empty deque");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "Pop_back on empty deque exception");
    }

    // Test 8: Sort
    std::cout << "\n### Sort ###\n";
    int sort_init_data[] = {3, 1, 2};
    DequeList<int> deq6(sort_init_data, 3);
    deq6.sort();
    int sorted_data[] = {1, 2, 3};
    assert_custom(checkResult(deq6, sorted_data, 3), "Sort");

    // Test 9: Sort на пустой
    DequeList<int> deq7;
    deq7.sort();
    assert_custom(deq7.empty(), "Sort on empty deque");
    
    int single_element_data_init[] = {1};
    DequeList<int> deq7_single(single_element_data_init, 1);
    deq7_single.sort();
    int single_data[] = {1};
    assert_custom(checkResult(deq7_single, single_data, 1), "Sort on single-element deque");

    // Test 10: Get_sub
    std::cout << "\n### Get_sub ###\n";
    DequeList<int> deq8(data, 3); 
    DequeList<int>* sub_deq = deq8.get_sub(1, 2);
    int sub_data[] = {2, 3};
    assert_custom(checkResult(*sub_deq, sub_data, 2), "Get_sub");
    delete sub_deq;

    // Test 11: Get_sub с невалидными индексами
    try {
        deq8.get_sub(3, 4);
        assert_custom(false, "Get_sub with out-of-range indices");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "Get_sub out-of-range exception");
    }
    try {
        deq8.get_sub(2, 1);
        assert_custom(false, "Get_sub with begin > end");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "Get_sub begin > end exception");
    }

    // Test 12: Concat
    std::cout << "\n### Concat ###\n";
    DequeList<int> deq9(data, 3);
    int deq10_init_data[] = {4};
    DequeList<int> deq10(deq10_init_data, 1);
    deq9.concat(deq10);
    int concat_data[] = {1, 2, 3, 4};
    assert_custom(checkResult(deq9, concat_data, 4), "Concat");

    // Test 13: Concat с пустым дэком
    DequeList<int> deq11(data, 3); 
    DequeList<int> empty_deq2;
    deq11.concat(empty_deq2);
    assert_custom(checkResult(deq11, data, 3), "Concat with empty deque");

    // Test 14: Map
    std::cout << "\n### Map ###\n";
    DequeList<int> deq12(data, 3);
    auto square = [](const int& x) { return x * x; };
    deq12.map(square);
    int mapped_data[] = {1, 4, 9};
    assert_custom(checkResult(deq12, mapped_data, 3), "Map");

    // Test 15: Reduce
    std::cout << "\n### Reduce ###\n";
    DequeList<int> deq13(data, 3);
    auto sum = [](const int& a, const int& b) { return a + b; };
    int reduce_result = deq13.reduce(sum, 0);
    assert_custom(reduce_result == 6, "Reduce"); // 0 + 1 + 2 + 3

    // Test 16: Where
    std::cout << "\n### Where ###\n";
    int where_init_data[] = {1, 2, 3, 4};
    DequeList<int> deq14(where_init_data, 4);
    auto is_even = [](const int& x) { return x % 2 == 0; };
    DequeList<int>* filtered = deq14.where(is_even);
    int filtered_data[] = {2, 4};
    assert_custom(checkResult(*filtered, filtered_data, 2), "Where");
    delete filtered;

    // Test 17: Iterator
    std::cout << "\n### Iterator ###\n";
    DequeList<int> deq15(data, 3);
    int iter_sum = 0;
    for (auto it = deq15.begin(); it != deq15.end(); ++it) {
        iter_sum += *it;
    }
    assert_custom(iter_sum == 6, "Iterator sum"); // 1 + 2 + 3

    // Test 18: Erase
    std::cout << "\n### Erase ###\n";
    DequeList<int> deq16(data, 3);
    auto it = deq16.begin();
    ++it;
    deq16.erase(it);
    int erased_data[] = {1, 3};
    assert_custom(checkResult(deq16, erased_data, 2), "Erase");

    // Test 19: Clear
    std::cout << "\n### Clear ###\n";
    DequeList<int> deq17(data, 3);
    deq17.clear(deq17.begin(), deq17.end());
    assert_custom(deq17.empty(), "Clear");

    // Test 20: Search
    std::cout << "\n### Search ###\n";
    int search_main_data[] = {1, 2, 3, 2, 3};
    DequeList<int> deq18(search_main_data, 5);
    int search_pattern_data[] = {2, 3};
    DequeList<int> pattern(search_pattern_data, 2);
    auto search_it = deq18.search(pattern);
    int search_data[] = {2, 3};
    DequeList<int> found;
    for (int i = 0; search_it != deq18.end() && i < 2; ++i, ++search_it) {
        found.push_back(*search_it);
    }
    assert_custom(checkResult(found, search_data, 2), "Search");

    // Test 21: Search без совпадений
    DequeList<int> deq19(data, 3);
    int search_nomatch_pattern_data[] = {4, 5};
    DequeList<int> pattern2(search_nomatch_pattern_data, 2);
    auto no_match_it = deq19.search(pattern2);
    assert_custom(no_match_it == deq19.end(), "Search with no match");

    // Test 22: Search с пустым дэком
    auto empty_search_it = deq19.search(empty_deq2); // empty_deq2 was defined earlier and is empty
    assert_custom(empty_search_it == deq19.begin(), "Search with empty pattern");
}

void testDequeArray(){
    std::cout << "\n==== Testing DequeArray ====\n";

    // Test 1
    std::cout << "\n### Default constructor ###\n";
    DequeArray<int> deq1;
    assert_custom(deq1.empty(), "DequeArray: Empty");
    assert_custom(deq1.length() == 0, "DequeArray: Length");

    // Test 2
    std::cout << "\n### Constructor ###\n";
    int data[] = {1, 2, 3};
    DequeArray<int> deq2(data, 3);
    assert_custom(deq2.length() == 3, "DequeArray: Constructor length");
    assert_custom(checkResult(deq2, data, 3), "DequeArray: Constructor values");

    // Test 3: Push_front и back
    std::cout << "\n### Push_front and push_back ###\n";
    DequeArray<int> deq3;
    deq3.push_front(1);
    deq3.push_back(2);
    deq3.push_front(0);
    int push_data[] = {0, 1, 2};
    assert_custom(checkResult(deq3, push_data, 3), "DequeArray: Push_front/back");

    // Test 4: Front and back
    std::cout << "\n### Front and back ###\n";
    assert_custom(deq3.front() == 0, "DequeArray: Front value");
    assert_custom(deq3.back() == 2, "DequeArray: Back value");

    // Test 5: Front/back on empty
    DequeArray<int> empty_deq;
    try {
        empty_deq.front();
        assert_custom(false, "DequeArray: Front on empty");
    } catch (Errors error) {
        assert_custom(error == EmptyContainer, "DequeArray: Front empty exception");
    }
    try {
        empty_deq.back();
        assert_custom(false, "DequeArray: Back on empty");
    } catch (Errors error) {
        assert_custom(error == EmptyContainer, "DequeArray: Back empty exception");
    }

    // Test 6: Pop_front and pop_back
    std::cout << "\n### Pop_front and pop_back ###\n";
    DequeArray<int> deq4(data, 3); 
    deq4.pop_front();
    deq4.pop_back();
    int pop_data[] = {2};
    assert_custom(checkResult(deq4, pop_data, 1), "DequeArray: Pop_front/back");

    // Test 7: Pop on empty
    DequeArray<int> deq5;
    try {
        deq5.pop_front();
        assert_custom(false, "DequeArray: Pop_front on empty");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "DequeArray: Pop_front exception");
    }
    try {
        deq5.pop_back();
        assert_custom(false, "DequeArray: Pop_back on empty");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "DequeArray: Pop_back exception");
    }

    // Test 9: Sort on empty
    DequeArray<int> deq7;
    deq7.sort();
    assert_custom(deq7.empty(), "DequeArray: Sort empty");
    
    int single_element_data_init_arr[] = {1};
    DequeArray<int> deq7_single_arr(single_element_data_init_arr, 1);
    deq7_single_arr.sort();
    int single_data[] = {1};
    assert_custom(checkResult(deq7_single_arr, single_data, 1), "DequeArray: Sort single");

    // Test 10: Get_sub
    std::cout << "\n### Get_sub ###\n";
    DequeArray<int> deq8(data, 3); 
    DequeArray<int>* sub_deq = deq8.get_sub(1, 2);
    int sub_data[] = {2, 3};
    assert_custom(checkResult(*sub_deq, sub_data, 2), "DequeArray: Get_sub");
    delete sub_deq;

    // Test 11: Get_sub invalid
    try {
        deq8.get_sub(3, 4);
        assert_custom(false, "DequeArray: Get_sub out-of-bounds");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "DequeArray: Get_sub out-of-range");
    }
    try {
        deq8.get_sub(2, 1);
        assert_custom(false, "DequeArray: Get_sub begin > end");
    } catch (Errors error) {
        assert_custom(error == IndexOutOfRange, "DequeArray: Get_sub inverted");
    }

    // Test 12: Concat
    std::cout << "\n### Concat ###\n";
    DequeArray<int> deq9(data, 3); 
    int deq10_init_data_arr[] = {4};
    DequeArray<int> deq10(deq10_init_data_arr, 1);
    deq9.concat(deq10);
    int concat_data[] = {1, 2, 3, 4};
    assert_custom(checkResult(deq9, concat_data, 4), "DequeArray: Concat");

    // Test 13: Concat with empty
    DequeArray<int> deq11(data, 3); 
    DequeArray<int> empty_deq2;
    deq11.concat(empty_deq2);
    assert_custom(checkResult(deq11, data, 3), "DequeArray: Concat with empty");

    // Test 14: Map
    std::cout << "\n### Map ###\n";
    DequeArray<int> deq12(data, 3);
    auto square = [](const int& x) { return x * x; };
    deq12.map(square);
    int mapped_data[] = {1, 4, 9};
    assert_custom(checkResult(deq12, mapped_data, 3), "DequeArray: Map");

    // Test 15: Reduce
    std::cout << "\n### Reduce ###\n";
    DequeArray<int> deq13(data, 3); 
    auto sum = [](const int& a, const int& b) { return a + b; };
    int reduce_result = deq13.reduce(sum, 0);
    assert_custom(reduce_result == 6, "DequeArray: Reduce");

    // Test 16: Where
    std::cout << "\n### Where ###\n";
    int where_init_data_arr[] = {1, 2, 3, 4};
    DequeArray<int> deq14(where_init_data_arr, 4);
    auto is_even = [](const int& x) { return x % 2 == 0; };
    DequeArray<int>* filtered = deq14.where(is_even);
    int filtered_data[] = {2, 4};
    assert_custom(checkResult(*filtered, filtered_data, 2), "DequeArray: Where");
    delete filtered;

    // Test 17: Iterator
    std::cout << "\n### Iterator ###\n";
    DequeArray<int> deq15(data, 3);
    int iter_sum = 0;
    for (auto it = deq15.begin(); it != deq15.end(); ++it) {
        iter_sum += *it;
    }
    assert_custom(iter_sum == 6, "DequeArray: Iterator sum");

    // Test 18: Erase
    std::cout << "\n### Erase ###\n";
    DequeArray<int> deq16(data, 3);
    auto it = deq16.begin();
    ++it;
    deq16.erase(it);
    int erased_data[] = {1, 3};
    assert_custom(checkResult(deq16, erased_data, 2), "DequeArray: Erase");

    // Test 19: Clear
    std::cout << "\n### Clear ###\n";
    DequeArray<int> deq17(data, 3); 
    deq17.clear(deq17.begin(), deq17.end());
    assert_custom(deq17.empty(), "DequeArray: Clear");

    // Test 20: Search
    std::cout << "\n### Search ###\n";
    int search_main_data_arr[] = {1, 2, 3, 2, 3};
    DequeArray<int> deq18(search_main_data_arr, 5);
    int search_pattern_data_arr[] = {2, 3};
    DequeArray<int> pattern(search_pattern_data_arr, 2);
    auto search_it = deq18.search(pattern);
    int search_data[] = {2, 3};
    DequeArray<int> found;
    for (int i = 0; search_it != deq18.end() && i < 2; ++i, ++search_it) {
        found.push_back(*search_it);
    }
    assert_custom(checkResult(found, search_data, 2), "DequeArray: Search");

    // Test 21: Search no match
    DequeArray<int> deq19(data, 3);
    int search_nomatch_pattern_data_arr[] = {4, 5};
    DequeArray<int> pattern2(search_nomatch_pattern_data_arr, 2);
    auto no_match_it = deq19.search(pattern2);
    assert_custom(no_match_it == deq19.end(), "DequeArray: Search no match");

    // Test 22: Search empty pattern
    auto empty_search_it = deq19.search(empty_deq2);
    assert_custom(empty_search_it == deq19.begin(), "DequeArray: Search empty pattern");
}

int runTests() {
    testDequeList();
    testDequeArray();
    std::cout << "All Deque tests completed successfully" << std::endl;
    return 0;
}