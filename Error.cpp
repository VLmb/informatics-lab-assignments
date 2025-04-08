#include "Errors.h"
#include <iostream>

const char* errors[5] = {
    "Operation completed successfully.",
    "Error: Index is out of range. Please provide a valid index.",
    "Error: Invalid argument provided. Please check the input values.",
    "Error: The container is empty. No elements to process.",
    "Error: Null pointer passed as an argument. Please provide a valid pointer."
};

void checkErrors(int errorIndex) {
    std::cout << errors[errorIndex] << std::endl;
}