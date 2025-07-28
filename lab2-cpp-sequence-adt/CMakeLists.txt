cmake_minimum_required(VERSION 3.10)

project(SequenceProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(SOURCES
    src/main.cpp
    src/Tests.cpp
    src/Interface.cpp
    src/CustomErrors.cpp
)

set(HEADERS
    src/Tests.h
    src/Interface.h
    src/CustomErrors.h
    src/DynamicArray.h
    src/LinkedList.h
    src/ArraySequence.h
    src/ListSequence.h
    src/ImmutableArraySequence.h
    src/ImmutableListSequence.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
