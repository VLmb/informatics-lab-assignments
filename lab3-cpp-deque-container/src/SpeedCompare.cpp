#include "SpeedCompare.h"
#include "x86intrin.h"
#include "RingDeque.h"

using clk = std::chrono::high_resolution_clock;
using ms = std::chrono::duration<double, std::milli>;

int SpeedCompare() {
    const size_t N = 20000;
    long long sum = 0;
    std::cout << "--- Compare of DequeArray, DequeList & RingDeque ---" << std::endl;
    std::cout << "Count of elements/operations (N): " << N << std::endl << std::endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    auto start_time = clk::now();
    auto end_time = clk::now();
    auto duration_us = ms(end_time - start_time);
    auto duration_ns = ms(end_time - start_time); 

    //1) Тест push_back
    std::cout << "#1 Testing push_back:" << std::endl;
    {
        DequeArray<int> dq_array_pb;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_array_pb.push_back(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_ns.count() << " mls" << std::endl;

        DequeList<int> dq_list_pb;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_list_pb.push_back(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_ns.count() << " mls" << std::endl;

        RingDeque<int> dq_ring_pb;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_ring_pb.push_back(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_ns.count() << " mls" << std::endl;
    }
    std::cout << std::endl;

    //2) Тест push_front
    std::cout << "#2 Testing push_front:" << std::endl;
    {
        DequeArray<int> dq_array_pf;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_array_pf.push_front(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_ns.count() << " mls" << std::endl;

        DequeList<int> dq_list_pf;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_list_pf.push_front(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_ns.count() << " mls" << std::endl;

        RingDeque<int> dq_ring_pf;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) dq_ring_pf.push_front(i);
        end_time = clk::now();
        duration_ns = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_ns.count() << " mls" << std::endl;
    }
    std::cout << std::endl;

    DequeArray<int> dq_array;
    DequeList<int> dq_list;
    RingDeque<int> dq_ring;
    for (size_t i = 0; i < N; ++i) {
        dq_array.push_back(i);
        dq_list.push_back(i);
        dq_ring.push_back(i);
    }

    //3 Тест front
    std::cout << "#3 Testing front:" << std::endl;
    {
        volatile int val;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
            if (!dq_array.empty()) val = dq_array.front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_us.count() << " mls " << std::endl;

        sum = 0;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
            if (!dq_list.empty()) val = dq_list.front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_us.count() << " mls" << std::endl;
        
        sum = 0;
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
            if (!dq_ring.empty()) val = dq_ring.front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_us.count() << " mls " << std::endl;
    }
    std::cout << std::endl;

    //4) Тест pop_back
    std::cout << "#4 Testing pop_back" << std::endl;
    {
        DequeArray<int> dq_arr_pop;
        DequeList<int> dq_lst_pop;
        RingDeque<int> dq_rg_pop;
        for(size_t k=0; k<N; ++k){
            dq_lst_pop.push_back(k);
            dq_arr_pop.push_back(k);
            dq_rg_pop.push_back(k);
        }
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
            if(!dq_arr_pop.empty()){ 
                sum += dq_arr_pop.back();
                dq_arr_pop.pop_back(); 
            }
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_us.count() << " mls " << std::endl;

        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
             if(!dq_lst_pop.empty()) { sum += dq_lst_pop.back(); dq_lst_pop.pop_back(); } else break;
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_us.count() << " mls " << std::endl;

        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) {
             if(!dq_rg_pop.empty()) { sum += dq_rg_pop.back(); dq_rg_pop.pop_back(); } else break;
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_us.count() << " mls " << std::endl;
    }
    std::cout << std::endl;

    //5) Тест pop_front
    std::cout << "#5 Testing pop_front" << std::endl;
    {
        DequeArray<int> dq_arr_pop;
        DequeList<int> dq_lst_pop;
        RingDeque<int> dq_rg_pop;
        for(size_t k=0; k<N; ++k){
            dq_lst_pop.push_back(k);
            dq_arr_pop.push_back(k);
            dq_rg_pop.push_back(k);
        }
        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) { 
            if(!dq_arr_pop.empty()) dq_arr_pop.pop_front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_us.count() << " mls" << std::endl;

        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) { 
            if(!dq_lst_pop.empty()) dq_lst_pop.pop_front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_us.count() << " mls" << std::endl;

        start_time = clk::now();
        for (size_t i = 0; i < N; ++i) { 
            if(!dq_rg_pop.empty()) dq_rg_pop.pop_front();
        }
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_us.count() << " mls" << std::endl;
    }
    std::cout << std::endl;
    
    //6) Тест get_sub
    std::cout << "#6 Testing get_sub:" << std::endl;
    if (N > 1) {
        int sub_start_index = N / 4;
        int sub_end_index = (N * 3) / 4;

            volatile Deque<int>* sub_array_ptr;
            start_time = clk::now();
            sub_array_ptr = dq_array.get_sub(sub_start_index, sub_end_index);
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeArray: " << duration_us.count() << std::endl;
            delete sub_array_ptr;

            volatile Deque<int>* sub_list_ptr;
            start_time = clk::now();
            sub_list_ptr = dq_list.get_sub(sub_start_index, sub_end_index);
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeList:  " << duration_us.count() << " mls " << std::endl;
            delete sub_list_ptr;

            volatile Deque<int>* sub_ring_ptr;
            start_time = clk::now();
            sub_ring_ptr = dq_ring.get_sub(sub_start_index, sub_end_index);
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   RingDeque:  " << duration_us.count() << " mls " << std::endl;
            delete sub_ring_ptr;

        }
    std::cout << std::endl;

    // 7) Тест concat
    std::cout << "#7 Testing concat:" << std::endl;
    if (N > 0) { 
        start_time = clk::now();
        dq_array.concat(dq_array);
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_us.count() << " mls " << std::endl;

        start_time = clk::now();
        dq_list.concat(dq_list);
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_us.count() << " mls " << std::endl;

        start_time = clk::now();
        dq_ring.concat(dq_ring);
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_us.count() << " mls " << std::endl;
    }
    std::cout << std::endl;

    //8) Testing sort
    std::cout << "#8 Testing sort:" << std::endl;
    if (N > 0) {
        DequeArray<int> dq_array_sort;
        srand(42); 
        for (size_t i = 0; i < N; ++i) dq_array_sort.push_back(rand());
        start_time = clk::now();
        dq_array_sort.sort();
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeArray: " << duration_us.count() << " mls" << std::endl;

        DequeList<int> dq_list_sort;
        srand(42); 
        for (size_t i = 0; i < N; ++i) dq_list_sort.push_back(rand());
        start_time = clk::now();
        dq_list_sort.sort();
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   DequeList:  " << duration_us.count() << " mls" << std::endl;

        RingDeque<int> dq_ring_sort;
        srand(42);
        for (size_t i = 0; i < N; ++i) dq_ring_sort.push_back(rand());
        start_time = clk::now();
        dq_ring_sort.sort();
        end_time = clk::now();
        duration_us = ms(end_time - start_time);
        std::cout << "   RingDeque:  " << duration_us.count() << " mls" << std::endl;
    } else {
        std::cout << "   Test sort skipped (N=0)." << std::endl;
    }
    std::cout << std::endl;

    //9) Тест search
    std::cout << "#9 Testing search (DequeArray/List only):" << std::endl;
    const size_t n_sub = N / 5;
        DequeArray<int> sub_dq_array_search;
        DequeList<int> sub_dq_list_search;
        for(size_t i = 0; i < n_sub; ++i) {
            sub_dq_array_search.push_back(dq_array.get_sub((N/2)+i, (N/2)+i+1)->front());
            sub_dq_list_search.push_back(dq_list.get_sub((N/2)+i, (N/2)+i+1)->front());
        }
        
        if (!sub_dq_array_search.empty()) {
            typename DequeArray<int>::DequeArrayIterator it_arr;
            start_time = clk::now();
            it_arr = dq_array.search(sub_dq_array_search);
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeArray: " << duration_us.count() << " mls" << std::endl;

            typename DequeList<int>::DequeListIterator it_list;
            start_time = clk::now();
            it_list = dq_list.search(sub_dq_list_search);
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeList:  " << duration_us.count() << " mls" << std::endl;
        }
    std::cout << std::endl;


    //10) Тест erase
    std::cout << "#10 Testing erase" << std::endl; 
    const size_t n_erase = N / 3; 
    if (n_erase > 0 && N > 0) {
        DequeArray<int> dq_array_for_erase_test; 
        for(size_t k=0; k<N; ++k) dq_array_for_erase_test.push_back(k);
        if (!dq_array_for_erase_test.empty()) {
            auto it_deq_arr = dq_array_for_erase_test.begin();
            for (size_t i = 0; i < N / 6 && it_deq_arr != dq_array_for_erase_test.end(); ++i) ++it_deq_arr; // Move iterator a bit
            
            start_time = clk::now();
            for (size_t i = 0; i < n_erase; ++i) {
                if (it_deq_arr == dq_array_for_erase_test.end() || dq_array_for_erase_test.empty()) break;
                it_deq_arr = dq_array_for_erase_test.erase(it_deq_arr);
            }
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeArray: " << duration_us.count() << " mls" << std::endl;
        }


        DequeList<int> dq_list_for_erase_test; 
        for(size_t k=0; k<N; ++k) dq_list_for_erase_test.push_back(k); 
        if(!dq_list_for_erase_test.empty()){
            auto it_deq_list = dq_list_for_erase_test.begin();
            for (size_t i = 0; i < N / 6 && it_deq_list != dq_list_for_erase_test.end(); ++i) ++it_deq_list; // Move iterator

            start_time = clk::now();
            for (size_t i = 0; i < n_erase; ++i) {
                if (it_deq_list == dq_list_for_erase_test.end() || dq_list_for_erase_test.empty()) break;
                it_deq_list = dq_list_for_erase_test.erase(it_deq_list);
            }
            end_time = clk::now();
            duration_us = ms(end_time - start_time);
            std::cout << "   DequeList:  " << duration_us.count() << " mls" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "----- END -----" << std::endl;
    return 0;
}