#include "SpeedCompare.h"
#include "x86intrin.h"

int SpeedCompare() {
    const size_t N = 20000;
    long long sum = 0;
    std::cout << "--- Compare of DequeArray & DequeList ---" << std::endl;
    std::cout << "Count of elements/operations (N): " << N << std::endl << std::endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    auto duration_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    //1) Тест push_back
    std::cout << "#1 Testing push_back:" << std::endl;
    {
        DequeArray<int> dqr;
        auto st1 = __rdtsc();
        for (size_t i = 0; i < N; ++i) {
            dqr.push_back(i);
        }
        auto ft1 = __rdtsc();
        std::cout <<  ft1 - st1 << std::endl;

        DequeList<int> dql;
        auto st2 = __rdtsc();
        for (size_t i = 0; i < N; ++i) {
            dql.push_back(i);
        }
        auto ft2 = __rdtsc();
        std::cout <<  ft2 - st2 << std::endl;

        DequeArray<int> dq_array;
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            dq_array.push_back(i);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration_s.count() << " nanosec" << std::endl;

        DequeList<int> dq_list;
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            dq_list.push_back(i);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration_s.count() << " nanosec" << std::endl;
    }
    std::cout << std::endl;

    //2) Тест push_front
    std::cout << "#2 Testing push_front:" << std::endl;
    {
        DequeArray<int> dq_array;
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            dq_array.push_front(i);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration_s.count() << " nanosec" << std::endl;

        DequeList<int> dq_list;
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            dq_list.push_front(i);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration_s = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration_s.count() << " nanosec" << std::endl;
    }
    std::cout << std::endl;

    DequeArray<int> base_dq_array;
    DequeList<int> base_dq_list;
    for (size_t i = 0; i < N; ++i) {
        base_dq_array.push_back(i);
        base_dq_list.push_back(i);
    }

    //3. Тест front
    std::cout << "#3 Testing front:" << std::endl;
    {
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            if (!base_dq_array.empty())
                sum += base_dq_array.front();
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            if (!base_dq_list.empty()) 
                sum += base_dq_list.front();
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    }
    std::cout << std::endl;

    //4) Тест pop_back
    std::cout << "#4 Testing pop_back" << std::endl;
    {
        DequeArray<int> dq_array_for_pop_test;
        for(size_t k=0; k<N; ++k) dq_array_for_pop_test.push_back(k);

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            if(!dq_array_for_pop_test.empty()){
                sum += dq_array_for_pop_test.back();
                dq_array_for_pop_test.pop_back();
            }
            else break;
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        DequeList<int> dq_list_for_pop_test;
        for(size_t k=0; k<N; ++k) dq_list_for_pop_test.push_back(k);

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
             if(!dq_list_for_pop_test.empty()) {
                sum += dq_list_for_pop_test.back();
                dq_list_for_pop_test.pop_back();
             }
             else break;
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "#X Testing pop_front" << std::endl;
    {
        DequeArray<int> dq_array_for_pop_front_test;
        for(size_t k=0; k<N; ++k) {
            dq_array_for_pop_front_test.push_back(static_cast<int>(k));
        }

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            if(!dq_array_for_pop_front_test.empty()) {
                dq_array_for_pop_front_test.pop_front();
            } else {
                break;
            }
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        DequeList<int> dq_list_for_pop_front_test;
        for(size_t k=0; k<N; ++k) {
            dq_list_for_pop_front_test.push_back(static_cast<int>(k));
        }

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
             if(!dq_list_for_pop_front_test.empty()) {
                dq_list_for_pop_front_test.pop_front();
             } else {
                break;
             }
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    }
    std::cout << std::endl;
    
    //5) Тест get_sub
    std::cout << "#5 Testing get_sub:" << std::endl;
    if (N > 1) {
        int begin_idx = N/4;
        int end_idx = (N * 3)/4 - 1;
        if (end_idx < begin_idx && N > 0) { 
            end_idx = begin_idx; 
            if (begin_idx >= N) { 
                 begin_idx = N > 0 ? 0 : 0;
                 end_idx = N > 0 ? 0 : -1; 
            }
        }
        if (N > 0 && end_idx < 0) end_idx = 0; 

        int sub_start_index = 0;
        int sub_end_index = -1;
        if (N > 0) {
            sub_start_index = N / 4;
            sub_end_index = (N * 3) / 4 - 1;
            if (sub_end_index < sub_start_index) { 
                if (N > 0) sub_end_index = sub_start_index;
                if (sub_start_index >= N) {
                    sub_start_index = 0;
                    sub_end_index = (N > 0) ? 0 : -1;
                }
            }
        }

        if (N > 0 && sub_end_index >= sub_start_index) { 
            DequeArray<int>* sub_array;
            start_time = std::chrono::high_resolution_clock::now();
            sub_array = base_dq_array.get_sub(sub_start_index, sub_end_index);
            end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;
            if (sub_array) delete sub_array;

            DequeList<int>* sub_list;
            start_time = std::chrono::high_resolution_clock::now();
            sub_list = base_dq_list.get_sub(sub_start_index, sub_end_index);
            end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
            if (sub_list) delete sub_list;
        } 
    } else { 
        std::cout << "   Test get_sub skipped (N <= 1)." << std::endl;
    }
    std::cout << std::endl;

    // 6) Тест concat
    std::cout << "#6 Testing concat:" << std::endl;
    if (N > 0) { 
        DequeArray<int> dq_array1, dq_array2;
        for (size_t i = 0; i < N / 2; ++i) {
            dq_array1.push_back(i);
            dq_array2.push_back(i + N / 2);
        }
        start_time = std::chrono::high_resolution_clock::now();
        dq_array1.concat(dq_array2);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration_s.count() << " microsec (lenght: " << dq_array1.length() << ")" << std::endl;

        DequeList<int> dq_list1, dq_list2;
        for (size_t i = 0; i < N / 2; ++i) {
            dq_list1.push_back(i);
            dq_list2.push_back(i + N / 2);
        }
        start_time = std::chrono::high_resolution_clock::now();
        dq_list1.concat(dq_list2);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec (length: " << dq_list1.length() << ")" << std::endl; // было "длина", а не lenght
    } else {
        std::cout << "   Test concat skipped (N=0)." << std::endl;
    }
    std::cout << std::endl;

    //7) Testing sort
    std::cout << "#7 Testing sort:" << std::endl;
    if (N > 0) {
        DequeArray<int> dq_array_sort;
        srand(42); 
        for (size_t i = 0; i < N; ++i) {
            dq_array_sort.push_back(rand());
        }
        start_time = std::chrono::high_resolution_clock::now();
        dq_array_sort.sort();
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        DequeList<int> dq_list_sort;
        srand(42); 
        for (size_t i = 0; i < N; ++i) {
            dq_list_sort.push_back(rand());
        }
        start_time = std::chrono::high_resolution_clock::now();
        dq_list_sort.sort();
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    } else {
        std::cout << "   Test sort skipped (N=0)." << std::endl;
    }
    std::cout << std::endl;
    
    auto map_func = [](const int& x){ return x * 2 + 1; };
    auto reduce_func = [](const int& a, const int& b){ return a + b; };
    auto where_func = [](const int& x){ return x % 3 == 0; };

    //8) Тест map
    std::cout << "#8 Testing map:" << std::endl;
    {
        DequeArray<int> dq_array_for_map_test; 
        for(size_t k=0; k<N; ++k) dq_array_for_map_test.push_back(k); 

        start_time = std::chrono::high_resolution_clock::now();
        if (!dq_array_for_map_test.empty()) dq_array_for_map_test.map(map_func);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        DequeList<int> dq_list_for_map_test; 
        for(size_t k=0; k<N; ++k) dq_list_for_map_test.push_back(k); 

        start_time = std::chrono::high_resolution_clock::now();
        if (!dq_list_for_map_test.empty()) dq_list_for_map_test.map(map_func);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    }
    std::cout << std::endl;

    //9) Тест reduce 
    std::cout << "#9 Testing reduce:" << std::endl;
    {
        volatile int res_array = 0;
        volatile int dummy_sum_for_compiler = 0; 
        start_time = std::chrono::high_resolution_clock::now();
        if (!base_dq_array.empty()) res_array = base_dq_array.reduce(reduce_func, 0);
        for (int k = 0; k < res_array % 100 + 1; ++k) { 
                dummy_sum_for_compiler += base_dq_array.front();
            }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec (result: " << res_array << ")" << std::endl;

        volatile int res_list = 0; 
        start_time = std::chrono::high_resolution_clock::now();
        if (!base_dq_list.empty()) res_list = base_dq_list.reduce(reduce_func, 0);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec (result: " << res_list << ")" << std::endl;
    }
    std::cout << std::endl;

    //10) Тест where
    std::cout << "#10 Testing where:" << std::endl;
    {
        DequeArray<int>* filtered_array = nullptr; 
        start_time = std::chrono::high_resolution_clock::now();
        if(!base_dq_array.empty()) filtered_array = base_dq_array.where(where_func);
        else filtered_array = new DequeArray<int>();
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec (length: " << (filtered_array ? filtered_array->length() : 0) << ")" << std::endl;
        if (filtered_array) delete filtered_array;

        DequeList<int>* filtered_list = nullptr; 
        start_time = std::chrono::high_resolution_clock::now();
        if(!base_dq_list.empty()) filtered_list = base_dq_list.where(where_func);
        else filtered_list = new DequeList<int>(); 
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec (lenght: " << (filtered_list ? filtered_list->length() : 0) << ")" << std::endl;
        if (filtered_list) delete filtered_list;
    }
    std::cout << std::endl;

    //11) Тест search
    std::cout << "#11 Testing search:" << std::endl;
    const size_t n_sub = 5;
    if (N >= n_sub && N > 0) { 
        DequeArray<int> sub_dq_array_search;
        DequeList<int> sub_dq_list_search;
        for(size_t i = 0; i < n_sub; ++i) {
            int value_to_search = (N / 2) + i;
            sub_dq_array_search.push_back(value_to_search);
            sub_dq_list_search.push_back(value_to_search);
        }

        typename DequeArray<int>::DequeArrayIterator it_arr;
        start_time = std::chrono::high_resolution_clock::now();
        if (!base_dq_array.empty()) it_arr = base_dq_array.search(sub_dq_array_search);
        else it_arr = base_dq_array.end();
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << std::endl;

        typename DequeList<int>::DequeListIterator it_list;
        start_time = std::chrono::high_resolution_clock::now();
        if (!base_dq_list.empty()) it_list = base_dq_list.search(sub_dq_list_search);
        else it_list = base_dq_list.end();
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << std::endl;
    }
    std::cout << std::endl;


    //12) Тест erase
    std::cout << "#12 Testing erase" << std::endl; 
    const size_t n_erase = N / 3; 
    if (n_erase > 0) {
        DequeArray<int> dq_array_for_erase_test; 
        for(size_t k=0; k<N; ++k) dq_array_for_erase_test.push_back(k);

        auto it_deq_arr = dq_array_for_erase_test.begin();
        for (size_t i = 0; i < N / 3; ++i){ 
            if (it_deq_arr != dq_array_for_erase_test.end()) ++it_deq_arr;
            else break;
        }
        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_erase; ++i) {
            if (it_deq_arr == dq_array_for_erase_test.end()) break;
            it_deq_arr = dq_array_for_erase_test.erase(it_deq_arr);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeArray: " << duration.count() << " microsec" << std::endl;

        DequeList<int> dq_list_for_erase_test; 
        for(size_t k=0; k<N; ++k) dq_list_for_erase_test.push_back(k); 

        auto it_deq_list = dq_list_for_erase_test.begin();
        for (size_t i = 0; i < N / 3; ++i){
             if (it_deq_list != dq_list_for_erase_test.end()) ++it_deq_list;
             else break;
        }

        start_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < n_erase; ++i) {
            if (it_deq_list == dq_list_for_erase_test.end()) break;
            it_deq_list = dq_list_for_erase_test.erase(it_deq_list);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "   DequeList:  " << duration.count() << " microsec" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----- END -----" << std::endl;
    return 0;
}