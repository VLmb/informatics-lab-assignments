#pragma once

#include <iostream>
#include <string>
#include "DequeList.h"
#include "DequeArray.h"
#include "Sequence/CustomErrors.h"

void assert_custom(bool condition, const std::string& testName);

template<typename T>
bool checkResult(DequeList<T>& deq, T* data, size_t size);
template<typename T>
bool checkResult(DequeArray<T>& deq, T* data, size_t size);

void testDequeList();
void testDequeArray();

int runTests();
