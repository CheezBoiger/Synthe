// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Allocator.hpp"


namespace Synthe {


//! Free List block structure, used by the Freelist allocator.
//! 
struct FreeListBlock
{
    U64             StartingAddress;
    U64             SizeInBytes;
    FreeListBlock*  PNext;
    B64             ShouldFree;
};


//! Free list allocation data structure.
//!
class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator()
        : m_Root(nullptr) { }

    ResultCode Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment) override;
    ResultCode Free(AllocationBlock* Block) override;

    void Reset() override;

protected:
    
    void OnInitialize() override;

private:
    FreeListBlock*  m_Root;
    U64             m_NumBlocks;
};
} // Synthe