#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <stdexcept>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"



void assert_custom(bool condition, const std::string& testName);

template<typename T>
bool checkResult(Sequence<T>& seq, T* data, size_t size);
template<typename T>
bool checkResult(DynamicArray<T>& array, T* data, size_t size);
template<typename T>
bool checkResult(LinkedList<T>& list, T* data, size_t size);

void testDynamicArray();     
void testLinkedList();       
void testMutableArraySequence(); 
void testImmutableArraySequence(); 
void testMutableListSequence();   
void testImmutableListSequence();

int runTests();

#endif