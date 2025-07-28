#include "UI.h"

void UI::Run() {
    while (true) {
        showMainMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
            case 1: createDeque(); break;
            case 2: switchDeque(); break;
            case 3:
                if (activeIndex >= 0 && activeIndex < static_cast<int>(deques.size())) { // Bounds check
                    deques[activeIndex]->menu();
                } else {
                    std::cout << "No active deque or invalid index\n";
                }
                break;
            case 0: return;
            default: std::cout << "Invalid option\n"; break;
        }
    }
}

void UI::showMainMenu() {
    std::cout << "\nMain Menu:\n"
              << "Active Deque: " << (activeIndex == -1 ? "None" : std::to_string(activeIndex)) << "\n"
              << "1. Create new deque\n"
              << "2. Switch to another deque\n"
              << "3. Operate on active deque\n"
              << "0. Exit\n"
              << "Choice: ";
}

void UI::createDeque() {
    int type_choice, dtype_choice;
    std::cout << "Container type: 1 - Array, 2 - List, 3 - Ring: "; // Modified
    std::cin >> type_choice;
    std::cout << "Data type: 1 - int, 2 - float, 3 - string, 4 - Person: ";
    std::cin >> dtype_choice;

    DequeType dequeType;
    if (type_choice == 1) {
        dequeType = DequeType::Array;
    } else if (type_choice == 2) {
        dequeType = DequeType::List;
    } else if (type_choice == 3) { // Added
        dequeType = DequeType::Ring;
    } else {
        std::cout << "Invalid container type\n";
        return;
    }

    switch (dtype_choice) {
        case 1:
            deques.push_back(new DequeContainer<int>(dequeType, this));
            break;
        case 2:
            deques.push_back(new DequeContainer<float>(dequeType, this));
            break;
        case 3:
            deques.push_back(new DequeContainer<std::string>(dequeType, this));
            break;
        case 4:
            deques.push_back(new DequeContainer<Person>(dequeType, this));
            break;
        default:
            std::cout << "Invalid data type\n";
            return;
    }

    activeIndex = static_cast<int>(deques.size()) - 1;
    std::cout << "Deque created and set as active (Index: " << activeIndex << ")\n";
}

void UI::switchDeque() {
    if (deques.empty()) {
        std::cout << "No deques available to switch.\n";
        return;
    }
    std::cout << "Existing deques (" << deques.size() << " total):\n";
    for (size_t i = 0; i < deques.size(); ++i) {
        // To show type, we'd need to store it in DequeContainerBase or use RTTI if DequeContainer knows its T
        // For now, just index and active status
        std::cout << i << (static_cast<int>(i) == activeIndex ? " [ACTIVE]" : "") << "\n";
    }
    std::cout << "Enter index to switch to: ";
    int index;
    std::cin >> index;
    if (index >= 0 && index < (int)deques.size()) {
        activeIndex = index;
        std::cout << "Switched to deque at index " << activeIndex << "\n";
    } else {
        std::cout << "Invalid index\n";
    }
}

template<typename T>
UI::DequeContainer<T>::DequeContainer(DequeType t, UI* o) : type(t), owner(o), deque(nullptr) { // Initialize deque to nullptr
    if (type == DequeType::Array)
        deque = new DequeArray<T>();
    else if (type == DequeType::List)
        deque = new DequeList<T>();
    else if (type == DequeType::Ring) 
        deque = new RingDeque<T>();
}

template<typename T>
UI::DequeContainer<T>::~DequeContainer() {
    delete deque;
}

template<typename T>
void UI::DequeContainer<T>::menu() {
    if (!deque) { // Safety check
        std::cout << "Error: Deque is not initialized.\n";
        return;
    }
    while (true) {
        std::cout << "\n--- Deque Operations Menu (Type: ";
        if (type == DequeType::Array) std::cout << "Array";
        else if (type == DequeType::List) std::cout << "List";
        else if (type == DequeType::Ring) std::cout << "Ring";
        std::cout << ", Data: " << typeid(T).name() << ") ---\n"
                  << "1. push_front\n2. push_back\n3. pop_front\n4. pop_back\n5. front\n6. back\n"
                  << "7. is_empty\n8. length\n9. sort\n10. map\n11. reduce\n12. where\n13. get_sub\n"
                  << "14. concat\n15. search (subsequence)\n16. clear all\n17. Print Deque\n"
                  << (type == DequeType::Ring ? "18. Special Print (RingDeque)\n" : "") // Conditional option
                  << "0. Back to Main Menu\nChoice: ";
        int choice;

        if (!(std::cin >> choice)) { 
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) return;

        T value;
        try {
            switch (choice) {
                case 1: // push_front
                case 2: // push_back
                    std::cout << "Enter value: ";
                    if constexpr (std::is_same_v<T, Person>) {
                        int ser, num;
                        std::string fn, mn, ln;
                        std::cout << "ID series number (e.g., 1234 567890): ";
                        std::cin >> ser >> num;
                        std::cout << "First Middle Last name (e.g., Ivan Ivanov Ivanovich): ";
                        std::cin >> fn >> mn >> ln;
                        std::cout << "Birthdate (year month day, e.g., 1990 1 15): ";
                        int y, m, d; std::cin >> y >> m >> d;
                        std::tm tm_struct = {}; // Initialize to zero
                        tm_struct.tm_year = y - 1900;
                        tm_struct.tm_mon = m - 1;
                        tm_struct.tm_mday = d;
                        time_t bdate = std::mktime(&tm_struct);
                        std::cout << "Role (0 - Student, 1 - Teacher): ";
                        int r_val; std::cin >> r_val;
                        value = Person{{ser, num}, fn, mn, ln, bdate, static_cast<Role>(r_val)};
                    } else {
                        std::cin >> value;
                        if (std::cin.fail()) { // Input validation for non-Person types
                             std::cout << "Invalid input for type.\n";
                             std::cin.clear();
                             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                             continue; // Skip operation
                        }
                    }
                    if (choice == 1) deque->push_front(value);
                    else deque->push_back(value);
                    std::cout << "Value pushed.\n";
                    break;
                case 3: deque->pop_front(); std::cout << "Popped from front.\n"; break;
                case 4: deque->pop_back(); std::cout << "Popped from back.\n"; break;
                case 5: std::cout << "Front: " << deque->front() << "\n"; break;
                case 6: std::cout << "Back: " << deque->back() << "\n"; break;
                case 7: std::cout << "Is empty: " << (deque->empty() ? "Yes" : "No") << "\n"; break;
                case 8: std::cout << "Length: " << deque->length() << "\n"; break;
                case 9: deque->sort(); std::cout << "Sorted\n"; break;
                case 10: // map
                    std::cout << "Applying map (e.g., +1 for numeric types, append ' mapped' for string)...\n";
                    deque->map([](const T& val) {
                        if constexpr (std::is_arithmetic_v<T>) return val + static_cast<T>(1);
                        else if constexpr (std::is_same_v<T, std::string>) return val + " mapped";
                        // else if constexpr (std::is_same_v<T, Person>) { Person p = val; p.middleName += "_mapped"; return p; } // Example for Person
                        else return val; // No change for other types like Person by default
                    });
                    std::cout << "Map applied.\n";
                    break;
                case 11: // reduce
                    if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
                        T init;
                        std::cout << "Enter initial value for reduce: ";
                        std::cin >> init;
                         if (std::cin.fail()) {
                             std::cout << "Invalid input for initial value.\n";
                             std::cin.clear();
                             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                             continue;
                        }
                        T result = deque->reduce([](const T& a, const T& b) { return a + b; }, init);
                        std::cout << "Reduce result: " << result << "\n";
                    } else {
                        std::cout << "Reduce (with sum) not directly supported for this data type via this menu option.\n";
                    }
                    break;
                case 12: { // where
                    std::cout << "Filtering (e.g., >0 for numeric, non-empty for string)...\n";
                    Deque<T>* filtered_deque_base = deque->where([](const T& val) {
                        if constexpr (std::is_arithmetic_v<T>) return val > static_cast<T>(0);
                        else if constexpr (std::is_same_v<T, std::string>) return !val.empty();
                        // else if constexpr (std::is_same_v<T, Person>) { return val.role == Role::Student; } // Example for Person
                        else return true; // Keep all for other types
                    });
                    // The 'where' method in DequeList/DequeArray returns DequeList*/DequeArray*
                    // The 'where' method in RingDeque returns Deque<T>* (actually RingDeque*)
                    // So, filtered_deque_base is of the correct dynamic type.
                    delete deque; // Delete old deque
                    deque = filtered_deque_base; // Assign new filtered deque
                    std::cout << "Filter applied. Current deque replaced with filtered version.\n";
                    break;
                }
                case 13: { // get_sub
                    int begin_idx, end_idx;
                    std::cout << "Enter start (inclusive) and end (exclusive) indices for subsequence: ";
                    std::cin >> begin_idx >> end_idx;
                    if (std::cin.fail() || begin_idx < 0 || end_idx < begin_idx) {
                        std::cout << "Invalid indices.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    Deque<T>* sub_deque_base = deque->get_sub(begin_idx, end_idx);
                    delete deque;
                    deque = sub_deque_base;
                    std::cout << "Subsequence extracted. Current deque replaced.\n";
                    break;
                }
                case 14: { // concat
                    std::cout << "Concat with another deque. It must be of the same data type (e.g. int, float) and same container type (Array, List, Ring).\n";
                    if (owner->deques.size() <= 1 && owner->deques[0] == this) {
                        std::cout << "No other deques available for concatenation.\n";
                        break;
                    }
                    std::cout << "Available deques to concatenate with:\n";
                    for (size_t i = 0; i < owner->deques.size(); ++i) {
                        if (owner->deques[i] == this) continue; // Don't list self
                        // We need to check if owner->deques[i] is DequeContainer<T> and matches 'this->type'
                        auto* other_potential_container = dynamic_cast<DequeContainer<T>*>(owner->deques[i]);
                        if (other_potential_container && other_potential_container->type == this->type) {
                             std::cout << i << " (Compatible type)\n";
                        } else {
                             std::cout << i << " (Incompatible type or data structure)\n";
                        }
                    }
                    std::cout << "Enter index of deque to concatenate with: ";
                    int concat_idx; std::cin >> concat_idx;

                    if (std::cin.fail() || concat_idx < 0 || concat_idx >= static_cast<int>(owner->deques.size()) || owner->deques[concat_idx] == this) {
                        std::cout << "Invalid index or cannot concat with self.\n";
                         if(std::cin.fail()){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}
                        break;
                    }
                    
                    DequeContainer<T>* other_container = dynamic_cast<DequeContainer<T>*>(owner->deques[concat_idx]);
                    if (other_container && other_container->type == this->type) {
                        if (this->type == DequeType::Array) {
                            auto* current_dq = dynamic_cast<DequeArray<T>*>(this->deque);
                            auto* other_dq = dynamic_cast<DequeArray<T>*>(other_container->deque);
                            if (current_dq && other_dq) current_dq->concat(*other_dq); else std::cout << "Concat error: Array type mismatch internally.\n";
                        } else if (this->type == DequeType::List) {
                            auto* current_dq = dynamic_cast<DequeList<T>*>(this->deque);
                            auto* other_dq = dynamic_cast<DequeList<T>*>(other_container->deque);
                            if (current_dq && other_dq) current_dq->concat(*other_dq); else std::cout << "Concat error: List type mismatch internally.\n";
                        } else if (this->type == DequeType::Ring) {
                            auto* current_dq = dynamic_cast<RingDeque<T>*>(this->deque);
                            auto* other_dq = dynamic_cast<RingDeque<T>*>(other_container->deque);
                            if (current_dq && other_dq) current_dq->concat(*other_dq); else std::cout << "Concat error: Ring type mismatch internally.\n";
                        }
                        std::cout << "Concatenation done.\n";
                    } else {
                        std::cout << "Selected deque is not compatible for concatenation (must be same data type AND same container type - Array, List, or Ring).\n";
                    }
                    break;
                }
                case 15: { // search
                    std::cout << "Search for a subsequence (provided by another deque of the same type and structure).\n";
                     if (owner->deques.size() <= 1 && owner->deques[0] == this) {
                        std::cout << "No other deques available to use as subsequence.\n";
                        break;
                    }
                    std::cout << "Available deques to use as subsequence:\n";
                     for (size_t i = 0; i < owner->deques.size(); ++i) {
                        if (owner->deques[i] == this) continue;
                        auto* other_potential_container = dynamic_cast<DequeContainer<T>*>(owner->deques[i]);
                        if (other_potential_container && other_potential_container->type == this->type) {
                             std::cout << i << " (Compatible type for subsequence)\n";
                        } else {
                             std::cout << i << " (Incompatible type for subsequence)\n";
                        }
                    }
                    std::cout << "Enter index of the deque to search for (the subsequence): ";
                    int idx_search_sub; std::cin >> idx_search_sub;

                    if (std::cin.fail() || idx_search_sub < 0 || idx_search_sub >= static_cast<int>(owner->deques.size()) || owner->deques[idx_search_sub] == this) {
                        std::cout << "Invalid index or cannot use self as subsequence.\n";
                        if(std::cin.fail()){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}
                        break;
                    }

                    DequeContainer<T>* subsequence_container = dynamic_cast<DequeContainer<T>*>(owner->deques[idx_search_sub]);
                    if (subsequence_container && subsequence_container->type == this->type) {
                        bool found = false;
                        if (this->type == DequeType::Array) {
                            auto* main_dq = dynamic_cast<DequeArray<T>*>(this->deque);
                            auto* sub_dq = dynamic_cast<DequeArray<T>*>(subsequence_container->deque);
                            if (main_dq && sub_dq) {
                                if (main_dq->search(*sub_dq) != main_dq->end()) found = true;
                            } else std::cout << "Search error: Array type mismatch internally.\n";
                        } else if (this->type == DequeType::List) {
                            auto* main_dq = dynamic_cast<DequeList<T>*>(this->deque);
                            auto* sub_dq = dynamic_cast<DequeList<T>*>(subsequence_container->deque);
                            if (main_dq && sub_dq) {
                                if (main_dq->search(*sub_dq) != main_dq->end()) found = true;
                            } else std::cout << "Search error: List type mismatch internally.\n";
                        } else if (this->type == DequeType::Ring) {
                            std::cout << "Subsequence search is not implemented for RingDeque in this menu.\n";
                        }

                        if (this->type == DequeType::Array || this->type == DequeType::List) {
                           if (found) std::cout << "Subsequence found.\n";
                           else std::cout << "Subsequence not found.\n";
                        }
                    } else {
                        std::cout << "Selected deque is not compatible for subsequence search.\n";
                    }
                    break;
                }
                case 16: { // clear all
                    if (this->type == DequeType::Array) {
                        auto* arr = dynamic_cast<DequeArray<T>*>(deque);
                        if (arr) arr->clear(arr->begin(), arr->end());
                    } else if (this->type == DequeType::List) {
                        auto* lst = dynamic_cast<DequeList<T>*>(deque);
                        if (lst) lst->clear(lst->begin(), lst->end());
                    } else if (this->type == DequeType::Ring) {
                        auto* ring_dq = dynamic_cast<RingDeque<T>*>(deque);
                        if (ring_dq) {
                            while (!ring_dq->empty()) {
                                ring_dq->pop_front(); // Or pop_back()
                            }
                        }
                    }
                    std::cout << "Deque cleared.\n";
                    break;
                }
                case 17: { // Print Deque
                    std::cout << "Current Deque Content: ";
                    if (deque->empty()) {
                        std::cout << "[empty]\n";
                    } else {
                        std::cout << "[ ";
                        if (this->type == DequeType::Array) {
                            auto* dq_arr = dynamic_cast<DequeArray<T>*>(deque);
                            if (dq_arr) {
                                for (auto it = dq_arr->begin(); it != dq_arr->end(); ++it) std::cout << *it << " ";
                            }
                        } else if (this->type == DequeType::List) {
                            auto* dq_list = dynamic_cast<DequeList<T>*>(deque);
                            if (dq_list) {
                                for (auto it = dq_list->begin(); it != dq_list->end(); ++it) std::cout << *it << " ";
                            }
                        } else if (this->type == DequeType::Ring) {
                            auto* dq_ring = dynamic_cast<RingDeque<T>*>(deque);
                            if (dq_ring) {
                                for (auto it = dq_ring->begin(); it != dq_ring->end(); ++it) std::cout << *it << " ";
                            }
                        }
                        std::cout << "]\n";
                    }
                    break;
                }
                case 18: // Special Print for RingDeque
                    if (this->type == DequeType::Ring) {
                        auto* dq_ring = dynamic_cast<RingDeque<T>*>(deque);
                        if (dq_ring) {
                            std::cout << "RingDeque internal structure (specialPrint):\n";
                            dq_ring->specialPrint();
                        } else {
                             std::cout << "Error: Not a RingDeque or not initialized.\n";
                        }
                    } else {
                        std::cout << "Invalid choice for non-RingDeque type.\n";
                    }
                    break;
                default: std::cout << "Invalid choice in operations menu.\n"; break;
            }
        } catch (const std::out_of_range& e) { // Catch specific exceptions like from Deque
            std::cout << "Error: Operation failed. " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "An unexpected error occurred: " << e.what() << "\n";
        } catch (...) { // Catch-all for other errors
            std::cout << "Error during operation. The deque might be empty or an invalid operation was attempted.\n";
        }
    }
}