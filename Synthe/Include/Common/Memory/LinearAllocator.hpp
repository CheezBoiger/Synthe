// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Allocator.hpp"


namespace Synthe {


class LinearAllocator : public Allocator {
public:
    LinearAllocator() : Allocator() { }

    void Initialize(UPtr RawMemRegion, U64 TotalSizeInBytes) override { }
    void Reset() override { m_Top = m_RawPtr; }
    
    B32 Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment) override { return false; }
    B32 Free(AllocationBlock* Ptr) override { return false; }

private:
    UPtr m_Top;
};
} // Synthe