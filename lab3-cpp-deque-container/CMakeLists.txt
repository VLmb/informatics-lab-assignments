cmake_minimum_required(VERSION 3.10)

project(Laborator-3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(SOURCES
    src/main.cpp
    src/Sequence/CustomErrors.cpp
    src/Test.cpp
    src/UI.cpp
    src/SpeedCompare.cpp
    src/TestingRingDeque.cpp
)

set(HEADERS
    src/Sequence/CustomErrors.h
    src/Sequence/LinkedList.h
    src/Sequence/ListSequence.h
    src/Sequence/ImmutableListSequence.h
    src/Sequence/Iterator.h
    src/Deque.h
    src/DequeList.h
    src/DequeArray.h
    src/Test.h
    src/UI.h
    src/SpeedCompare.h
    src/RingDeque.h
    src/TestingRingDeque.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
