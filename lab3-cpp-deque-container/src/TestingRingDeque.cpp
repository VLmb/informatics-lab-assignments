#include "TestingRingDeque.h"
#include "Test.h"

template<typename T>
bool checkResult(RingDeque<T>& deq, T* data, size_t size) {
    if (deq.length() != size) return false;
    try {
        auto it = deq.begin();
        for (size_t i = 0; i < size; ++i, ++it) {
            if (*it != data[i]){
                return false;
            }
        }
    } catch (Errors error) {
        std::cout << "Error in checkResult: " << error << "\n";
        return false;
    }
    return true;
}

void testRingDeque() {
    std::cout << "\n==== Testing RingDeque ====\n";

    // Test 1
    std::cout << "\n### Default constructor ###\n";
    RingDeque<int> deq1;
    assert_custom(deq1.empty(), "RingDeque: Empty");
    assert_custom(deq1.length() == 0, "RingDeque: Length");

    // Test 2
    std::cout << "\n### Constructor ###\n";
    int data[] = {1, 2, 3, 4, 5};
    RingDeque<int> deq2(data, 5);
    assert_custom(checkResult(deq2, data, 5), "RingDeque: Constructor length");

    // Test 3 push_front / push_back
    std::cout << "\n### Push_front and push_back ###\n";
    RingDeque<int> deq3;
    int td_1[3] = {0, 1, 2};
    deq3.push_front(1);
    deq3.push_back(2);
    deq3.push_front(0);
    assert_custom(checkResult(deq3, td_1, 3), "RingDeque: Push_front/front");


    // Test 4 pop_front / pop_back
    std::cout << "\n### Pop_front and pop_back ###\n";
    RingDeque<int> deq4(data, 5);
    int td_2[3] = {2, 3, 4};
    deq4.pop_front();
    deq4.pop_back();
    assert_custom(checkResult(deq4, td_2, 3), "RingDeque: Pop_front/back");

    int pop_all_data[] = {10, 20, 30};
    RingDeque<int> deq_pop_all(pop_all_data, 3);
    deq_pop_all.pop_back();  // {10, 20}
    deq_pop_all.pop_front(); // {20}
    deq_pop_all.pop_back();  // {}
    assert_custom(deq_pop_all.empty(), "RingDeque: Empty after pops");
    assert_custom(deq_pop_all.length() == 0, "RingDeque: Length 0 after pops");
    deq_pop_all.push_back(100);
    deq_pop_all.push_front(50); // {50, 100}
    int td_x[] = {50, 100};
    assert_custom(checkResult(deq_pop_all, td_x, 2), "RingDeque: Refill after emptying");

    // Test 5 get_sub
    std::cout << "\n### Get_sub ###\n";
    RingDeque<int> deq5(data, 5);
    Deque<int>* sub = deq5.get_sub(1, 3);
    RingDeque<int>* sub_ring = dynamic_cast<RingDeque<int>*>(sub);
    int td_3[2] = {2, 3};
    assert_custom(checkResult(*sub_ring, td_3, 2), "RingDeque: Get_sub");

    std::cout << "\n### Get_sub edge cases ###\n";
    RingDeque<int> deq_sub_wrap(4); 
    deq_sub_wrap.push_back(10); 
    deq_sub_wrap.push_back(20); 
    deq_sub_wrap.push_back(30); 
    deq_sub_wrap.pop_front();   
    deq_sub_wrap.push_back(40);
    deq_sub_wrap.push_back(50);  
    // Logical order: 20, 30, 40, 50
    int sub_d[] = {30, 40, 50};
    Deque<int>* sub_dq = deq_sub_wrap.get_sub(1, 3); 
    RingDeque<int>* sub_rgdq = dynamic_cast<RingDeque<int>*>(sub_dq);
    if (sub_rgdq) {
         assert_custom(checkResult(*sub_rgdq, sub_d, 2), "RingDeque: Get_sub from grow deque");
    }
    delete sub_rgdq;
    
    RingDeque<int> deq7(data, 5);
    Deque<int>* sub_dq_empty = deq7.get_sub(1, 1);
    RingDeque<int>* sub_rgdq_empty = dynamic_cast<RingDeque<int>*>(sub_dq_empty);
     if (sub_rgdq_empty) {
        assert_custom(sub_rgdq_empty->empty(), "RingDeque: Get_sub empty (begin == end)");
    }
    delete sub_rgdq_empty;

    Deque<int>* sub_dq_full = deq7.get_sub(0, deq7.length());
    RingDeque<int>* sub_rgdq_full = dynamic_cast<RingDeque<int>*>(sub_dq_full);
    if (sub_rgdq_full) {
        assert_custom(checkResult(*sub_rgdq_full, data, 5), "RingDeque: Get_sub full deque");
    }
    delete sub_dq_full;

    // Test 6 map
    std::cout << "\n### Map ###\n";
    RingDeque<int> deq6(data, 5);
    int td_4[5] = {1, 4, 9, 16, 25};
    auto square = [](const int& x) { return x * x; };
    deq6.map(square);
    assert_custom(checkResult(deq6, td_4, 5), "RingDeque: Map square");

    RingDeque<int> deq_empty;
    deq_empty.map(square); 
    assert_custom(deq_empty.empty(), "RingDeque: Map on empty deque");

    RingDeque<int> deq8;
    deq8.push_back(1); 
    deq8.push_back(2); 
    deq8.push_back(3); 
    deq8.pop_front(); 
    deq8.push_back(4);
    deq8.push_back(5);  // Order: 2,3,4
    deq8.map(square); 
    int td_8[] = {4, 9, 16, 25};
    assert_custom(checkResult(deq8,td_8, 4), "RingDeque: Map on wrapped deque");

    // Test 7 reduce
    std::cout << "\n### Reduce ###\n";
    RingDeque<int> deq9(data, 3);
    auto sum = [](const int& a, const int& b) { return a + b; };
    int result = deq9.reduce(sum, 0);
    assert_custom(result == 6, "RingDeque: Reduce");

    //Test 9 grow 
    std::cout << "\n### Grow test ###\n";
    RingDeque<int> deq_gw(3); 
    deq_gw.push_back(1);   
    deq_gw.push_back(2);   
    deq_gw.pop_front();    
    deq_gw.push_back(3);   
    deq_gw.push_back(4);
    int grow_wrap_expected[] = {2, 3, 4};
    assert_custom(checkResult(deq_gw, grow_wrap_expected, 3), "RingDeque: Grow with elements");

    //Test 10 where
    std::cout << "\n### Where ###\n";
    RingDeque<int> deq10(data, 5); // {1,2,3,4,5}
    auto is_even = [](const int& x) { return x % 2 == 0; };
    
    Deque<int>* where_res_ptr = deq10.where(is_even);
    RingDeque<int>* where_res_ring = dynamic_cast<RingDeque<int>*>(where_res_ptr);
    if (where_res_ring) {
        int where_even_expected[] = {2, 4};
        assert_custom(checkResult(*where_res_ring, where_even_expected, 2), "RingDeque: Where is_even");
    }
    delete where_res_ptr;
    assert_custom(checkResult(deq10, data, 5), "RingDeque: Where original deque unchanged");

    //Test 11 sort
    std::cout << "\n### Sort ###\n";
    RingDeque<int> deq11;
    int td_11[] = {5, 1, 4, 2, 8};
    int tds_11[] = {1, 2, 4, 5, 8};


    deq11 = RingDeque<int>(td_11, 5);
    // deq11.specialPrint();
    deq11.sort();
    assert_custom(checkResult(deq11, tds_11, 5), "RingDeque: Sort general case");
    // deq11.specialPrint();

    RingDeque<int> deq_s_empty;
    deq_s_empty.sort();
    assert_custom(deq_s_empty.empty(), "RingDeque: Sort empty deque");

    RingDeque<int> deq_s_one;
    deq_s_one.push_back(42);
    deq_s_one.sort();
    int s_one_sorted[] = {42};
    assert_custom(checkResult(deq_s_one, s_one_sorted, 1), "RingDeque: Sort single element deque");

    RingDeque<int> deq_s_already_sorted(tds_11, 5);
    deq_s_already_sorted.sort();
    assert_custom(checkResult(deq_s_already_sorted, tds_11, 5), "RingDeque: Sort already sorted deque");

    RingDeque<int> deq_s_grow(5); 
    deq_s_grow.push_back(30); 
    deq_s_grow.push_back(10); 
    deq_s_grow.push_back(40); 
    deq_s_grow.push_back(20); 
    deq_s_grow.pop_front();   
    deq_s_grow.push_back(50); // Order: 10,40,20,50
    deq_s_grow.sort();
    int s_wrap_sorted[] = {10, 20, 40, 50};
    assert_custom(checkResult(deq_s_grow, s_wrap_sorted, 4), "RingDeque: Sort wrapped deque");

    //Test 12 concat
    std::cout << "\n### Concat ###\n";
    int c1_data[] = {1,2};
    RingDeque<int> deq_c1(c1_data, 2);
    int c2_data[] = {3,4,5};
    RingDeque<int> deq_c2(c2_data, 3);
    deq_c1.concat(deq_c2); 
    int c1_expected[] = {1,2,3,4,5};
    assert_custom(checkResult(deq_c1, c1_expected, 5), "RingDeque: Concat two non-empty");
    assert_custom(checkResult(deq_c2, c2_data, 3), "RingDeque: Concat other deque unchanged");

    RingDeque<int> deq_c3;
    int c4_data[] = {6,7};
    RingDeque<int> deq_c4(c4_data, 2);
    deq_c3.concat(deq_c4); 
    assert_custom(checkResult(deq_c3, c4_data, 2), "RingDeque: Concat non-empty to empty");

    int c5_data[] = {8,9};
    RingDeque<int> deq_c5(c5_data, 2);
    RingDeque<int> deq_c6; 
    deq_c5.concat(deq_c6); 
    assert_custom(checkResult(deq_c5, c5_data, 2), "RingDeque: Concat empty to non-empty");
    
    RingDeque<int> deq_c7, deq_c8; 
    deq_c7.concat(deq_c8);
    assert_custom(deq_c7.empty(), "RingDeque: Concat two empty deques");

    std::cout << "All RingDeque tests passed!\n";
}