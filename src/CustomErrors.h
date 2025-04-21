#ifndef ERROR_H
#define ERROR_H

typedef enum {
    OK = 0,
    IndexOutOfRange = 1,
    NotValidArgument = 2,
    EmptyContainer = 3,
    NullPointerPassedAsArgument = 4,
    NotImplement = 5,
    ChangeImmutableType = 6
} Errors;

extern const char* errors[];
void checkErrors(int errorIndex);

#endif