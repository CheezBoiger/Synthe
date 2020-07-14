// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

#include <stdlib.h>


#define MEM_BYTES(B) (Synthe::U64)(B)
#define MEM_1KB (1024ULL)
#define MEM_1MB (1024ULL * 1024ULL)
#define MEM_1GB (1024ULL * 1024ULL * 1024ULL)
#define ALIGN_BYTES(Ptr, Alignment) ((Ptr) + ((Alignment) - 1)) & (~((Alignment) - 1))


namespace Synthe {

typedef U8 MemT;

struct AllocationBlock
{
    U64 StartAddress;
    U64 SizeInBytes;
    U32 AllocationID;
    U32 AllocatorPoolID;
};


template<typename Type, typename... Arguments>
static Type* Malloc(Arguments... Args)
{
    return new Type(Args...);
}


template<typename Type, typename... Arguments>
static Type* MallocArray(U64 Count)
{
    return new Type[Count];
}


template<typename Type>
static void Free(Type* MPtr)
{
    delete MPtr;
}


template<typename Type>
static void FreeArray(Type* MPtr)
{
    delete[] MPtr;
}


//! Generic Allocator Device, written agnostic for CPU and GPU memory 
//! data structures.
class Allocator {
public:
    typedef U64 UPtr;
    Allocator()
        : m_TotalSizeInBytes(0ULL)
        , m_CurrentUsedBytes(0ULL)
        , m_NumAllocations(0ULL)
        , m_ID(0ULL)
        , m_BaseAddress(0ULL)
    { }

    virtual ~Allocator() { }
   
    //! Initialization function for set up of allocator.
    void Initialize(UPtr BaseAddress, U64 TotalSizeInBytes) { 
        m_BaseAddress = BaseAddress;
        m_TotalSizeInBytes = TotalSizeInBytes;
        m_CurrentUsedBytes = 0ULL;
        m_NumAllocations = 0ULL;
        m_ID = static_cast<U32>(0); //< I guess we can use the base address as our ID.
        OnInitialize();
    }

    //! Reset the structure, this will clear out and invalidate all allocation blocks from the 
    //! memory handle.
    virtual void Reset() { }

    //! Allocate a block of memory.
    //! 
    //! \param Block
    //! \param SizeInBytes
    //! \param Alignment
    //! \return GResult_OK if the allocation was made. Any other error code if allocation fails. Block will not 
    //!         return initialized data if the function fails.
    virtual ResultCode Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment) { 
        return SResult_NOT_IMPLEMENTED; 
    }
    
    //! Free an allocation block handle. 
    //! 
    //! \param Ptr 
    //! \return GResult_OK if the function successfully frees the given memory block. Otherwise,
    //!         Should the function fail, this could indicate that the given block is 
    //!         invalid, is out of bounds of the memory block, or fails outright to clean up.
    virtual ResultCode Free(AllocationBlock* Ptr) { 
        return SResult_NOT_IMPLEMENTED; 
    }

    //! Get Total size of the allocator controlled memory.
    U64 GetTotalSizeBytes() const { return m_TotalSizeInBytes; }
    
    //! Get the current amount of memory used by this allocator. 
    U64 GetCurrentUsedBytes() const { return m_CurrentUsedBytes; }

    //! Get the number of successful allocation calls made for this allocator.
    U64 GetNumberOfAllocations() const { return m_NumAllocations; }

    //! Get the unique ID of the allocator.
    U32 GetID() const { return m_ID; }

protected:

    virtual void OnInitialize() { }

    //! The starting memory block pointer, or the base.
    UPtr        m_BaseAddress;

    //! The total size of the memory block that the allocator is in control of.
    U64         m_TotalSizeInBytes;

    //! Bytes used currently by this allocator, that have not been freed yet.
    U64         m_CurrentUsedBytes;

    //! Number of allocations made by this allocator.
    U64         m_NumAllocations;
    
    //! Allocator ID.
    U32         m_ID;
};
} // Synthe