#include "CustomErrors.h"
#include <iostream>

const char* errors[7] = {
    "Operation completed successfully.",
    "Error: Index is out of range. Please provide a valid index.",
    "Error: Invalid argument provided. Please check the input values.",
    "Error: The container is empty. No elements to process.",
    "Error: Null pointer passed as an argument. Please provide a valid pointer.",
    "Error: This method haven't implemented in the class from which you are calling it.",
    "Error: An attempt to change an immutable object."
};

void checkErrors(int errorIndex) {
    std::cout << errors[errorIndex] << std::endl;
}