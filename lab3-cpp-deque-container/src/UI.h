#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <limits> // Required for std::numeric_limits
#include "DequeArray.h"
#include "DequeList.h"
#include "RingDeque.h" // Added
#include "Person.h"

// enum class DequeType { Array, List }; // Original
enum class DequeType { Array, List, Ring }; // Modified

class UI {
public:
    void Run();

private:
    struct DequeContainerBase {
        virtual ~DequeContainerBase() {}
        virtual void menu() = 0;
    };

    template<typename T>
    struct DequeContainer : public DequeContainerBase {
        Deque<T>* deque;
        DequeType type;
        UI* owner;

        DequeContainer(DequeType type, UI* owner);
        ~DequeContainer();
        void menu() override;
    };

    std::vector<DequeContainerBase*> deques;
    int activeIndex = -1;

    void showMainMenu();
    void createDeque();
    void switchDeque();
};

template struct UI::DequeContainer<int>;
template struct UI::DequeContainer<float>;
template struct UI::DequeContainer<std::string>;
template struct UI::DequeContainer<Person>;