//
// Created by scott on 9/22/2023.
//

#include "shmo/errors.h"

static const char* ERROR_STRINGS[] = {
        [ERROR_NONE] = "No error",
        [ERROR_NULL_POINTER] = "Null pointer encountered",
        [ERROR_ALLOCATION_FAILED] = "Memory allocation failed",
        [ERROR_INDEX_OUT_OF_BOUNDS] = "Index out of bounds",
        [ERROR_KEY_NOT_FOUND] = "Key not found",
        [ERROR_INVALID_ARGUMENT] = "Invalid argument",
        [ERROR_OVERFLOW] = "Overflow occurred",
        [ERROR_UNDERFLOW] = "Underflow occurred",
        [ERROR_DIVISION_BY_ZERO] = "Division by zero attempted",
        [ERROR_CONVERSION_FAILED] = "Type conversion failed",
        [ERROR_RESOURCE_UNAVAILABLE] = "Resource unavailable",
        [ERROR_IO] = "Input/Output error",
        [ERROR_PERMISSION_DENIED] = "Permission denied",
        [ERROR_TIMEOUT] = "Operation timed out",
        [ERROR_UNKNOWN] = "Unknown error"
};

const char* error_string(ErrorCode code) {
    if (code < sizeof(ERROR_STRINGS) / sizeof(ERROR_STRINGS[0])) {
        return ERROR_STRINGS[code];
    }
    return "Invalid error code";
}
