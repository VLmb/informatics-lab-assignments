#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"

// Start UI stuff
void runUI();

// Show options
void showMenu();

// Seq ops
Sequence<int>* chooseSequence();         // Pick seq type
int doAppend(Sequence<int>*& seq);      // Add to end
int doPrepend(Sequence<int>*& seq);     // Add to start
int doInsert(Sequence<int>* seq);       // Insert at index
int doConcat(Sequence<int>*& seq);      // Merge with another
int doGetSub(Sequence<int>* seq);       // Grab a chunk
int doMap(Sequence<int>*& seq);         // Square everything
int doReduce(Sequence<int>* seq);       // Sum it up
void doPrint(Sequence<int>* seq);        // Show seq
int doSet(Sequence<int>* seq);          // Set value (mutable only)
int doSwitch(Sequence<int>*& seq);

#endif // INTERFACE_H