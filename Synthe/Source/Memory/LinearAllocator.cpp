// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "Common/Memory/LinearAllocator.hpp"

namespace Synthe {



void LinearAllocator::OnInitialize()
{
    Reset();
}


ResultCode LinearAllocator::Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment)
{
    U64 LastPtr = m_BaseAddress + m_TotalSizeInBytes;
    U64 FuturePtr = ALIGN_BYTES(m_Top + SizeInBytes, Alignment);
    if (FuturePtr >= LastPtr)
    {
        return SResult_OUT_OF_BOUNDS;
    }

    if (!Block)
    {
        return SResult_INITIALIZATION_FAILURE;
    }

    Block->Start = m_Top;
    Block->Offset = SizeInBytes;
    Block->AllocationID = static_cast<U32>(m_NumAllocations++);
    Block->AllocatorPoolID = m_ID;

    m_CurrentUsedBytes += SizeInBytes;
    return SResult_OK;
}
} // Synthe