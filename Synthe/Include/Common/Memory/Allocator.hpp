// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

namespace Synthe {


struct AllocationBlock
{
    U64 Start;
    U64 Offset;
    U32 AllocationID;
    U32 AllocatorPoolID;
};


// Generic Allocator Device, written agnostic for CPU and GPU memory 
// data structures.
class Allocator {
public:
    typedef U64 UPtr;
    Allocator()
        : m_TotalSizeInBytes(0ull)
        , m_CurrentUsedBytes(0ull)
        , m_NumAllocations(0ull)
    { }

    virtual ~Allocator() { }
    virtual void Initialize(UPtr RawMemRegion, U64 TotalSizeInBytes) { }
    virtual void Reset() { }

    virtual B32 Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment) { }
    virtual B32 Free(AllocationBlock* Ptr) { }
protected:
    UPtr m_RawPtr;
    U64 m_TotalSizeInBytes;
    U64 m_CurrentUsedBytes;
    U64 m_NumAllocations;
};
} // Synthe