#pragma once

#include <iostream>
#include "RingDeque.h"
#include "Sequence/CustomErrors.h"

void assert_custom(bool condition, const std::string& testName);
void testRingDeque();