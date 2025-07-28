#pragma once

#include <iostream>
#include <string>
#include "Sequence/DynamicArray.h"
#include "Sequence/CustomErrors.h"

void assert_custom(bool condition, const std::string& testName);

void testAVLTree();

void runTests();
