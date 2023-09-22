//
// Created by scott on 9/22/2023.
//

#ifndef SHMOCODEBASE_ERRORS_H
#define SHMOCODEBASE_ERRORS_H

typedef enum ErrorCode {
    ERROR_NONE = 0,             // No error occurred
    ERROR_NULL_POINTER,         // A null pointer was encountered
    ERROR_ALLOCATION_FAILED,    // Memory allocation failed
    ERROR_INDEX_OUT_OF_BOUNDS,  // Index is out of valid range
    ERROR_KEY_NOT_FOUND,        // The specified key was not found
    ERROR_INVALID_ARGUMENT,     // An invalid argument was provided
    ERROR_OVERFLOW,             // An overflow occurred during an operation
    ERROR_UNDERFLOW,            // An underflow occurred during an operation
    ERROR_DIVISION_BY_ZERO,     // Division by zero attempted
    ERROR_CONVERSION_FAILED,    // Conversion between types failed
    ERROR_RESOURCE_UNAVAILABLE, // The requested resource is unavailable
    ERROR_IO,                   // An Input/Output error occurred
    ERROR_PERMISSION_DENIED,    // Permission denied for the requested operation
    ERROR_TIMEOUT,              // The operation timed out
    ERROR_UNKNOWN               // An unknown error occurred
} ErrorCode;

const char* error_string(ErrorCode code);

#endif //SHMOCODEBASE_ERRORS_H
