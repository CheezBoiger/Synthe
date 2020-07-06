// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include <cstdint>
#include <string>

namespace Synthe {

typedef char I8;
typedef unsigned char U8;

typedef short I16;
typedef unsigned short U16;

typedef int I32;
typedef unsigned int U32;

typedef long long I64;
typedef unsigned long long U64;

typedef float R32;
typedef double R64;

typedef size_t SIZE;
typedef I32 B32;
typedef I64 B64;
typedef I8 B8;

typedef I32 ResultCode;

enum SResult
{
    SResult_OK = 1,
    SResult_FAILED = 0,
    SResult_NOT_IMPLEMENTED = -99999,
    SResult_BAD_CAST,
    SResult_NOT_INITIALIZED, 
    SResult_OUT_OF_BOUNDS,
    SResult_INVALID_CALL,
    SResult_INVALID_ARGS,
    SResult_INITIALIZATION_FAILURE,
    SResult_MEMORY_ALLOCATION_FAILURE,
    SResult_OUT_OF_MEMORY,
    SResult_REFUSE_CALL,
    SResult_MEMORY_NULL_EXCEPTION,
    SResult_NOT_AVAILABLE,
    SResult_OBJECT_NOT_FOUND,
    SResult_MEMORY_CORRUPTION,
    SResult_ALREADY_EXISTS
};
} // Synthe