cmake_minimum_required(VERSION 3.10)

project(Laborator-4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/Test.cpp
    src/Compare.cpp
    src/UI.cpp
    src/Sequence/CustomErrors.cpp
)

set(HEADERS
    src/AVLTree.h
    src/BST.h
    src/Compare.h
    src/Test.h
    src/UI.h
    src/Person.h
    src/Sequence/CustomErrors.h
    src/Sequence/DynamicArray.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
