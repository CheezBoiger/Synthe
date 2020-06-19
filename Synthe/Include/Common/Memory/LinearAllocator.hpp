// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Allocator.hpp"


namespace Synthe {


//! Linear Allocator is a stack based allocator, which is mainly for allocating and
//! placing the requested memory block on top of the last allocated block. This is 
//! fast as it does not need to make many checks other than bounds. With an already 
//! defined memory block initialized/malloc'ed, this process is very fast. A top pointer 
//! determines the top of the stack.
class LinearAllocator : public Allocator {
public:
    LinearAllocator() 
        : m_Top(MEM_BYTES(0))
        , Allocator() { }

    void OnInitialize() override;
    
    //! Reset the linear allocator.
    void Reset() override { 
        m_Top = m_BaseAddress; 
        m_NumAllocations = 0ULL;  
        m_CurrentUsedBytes = 0ULL;
    }
    
    //!
    ResultCode Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment) override;
    
    //! 
    ResultCode Free(AllocationBlock* Ptr) override { return SResult_NOT_AVAILABLE; }

private:
    UPtr    m_Top;
};
} // Synthe