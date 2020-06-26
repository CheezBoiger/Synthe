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
    // Expand needed bytes for alignment, to use on aligning the address.
    // Find the padding of the requested Size, and apply to the Expanded/Needed Size.
    U64 Padding = (Alignment - (SizeInBytes & (Alignment - 1))) & (Alignment - 1);
    U64 NeededBytes = (SizeInBytes + (Alignment - 1)) & ~(Alignment - 1);
    // Just calculate the last address bound of the memory block.
    U64 LastPtr = m_BaseAddress + m_TotalSizeInBytes;
    U64 FuturePtr = m_Top + NeededBytes;
    if (FuturePtr >= LastPtr)
    {
        return SResult_OUT_OF_BOUNDS;
    }

    if (!Block)
    {
        return SResult_INITIALIZATION_FAILURE;
    }

    Block->StartAddress = ALIGN_BYTES(m_Top, Alignment);
    Block->SizeInBytes = SizeInBytes;
    Block->AllocationID = static_cast<U32>(m_NumAllocations++);
    Block->AllocatorPoolID = m_ID;

    // Append the next available address based on the expanded bytes.
    m_Top += NeededBytes;
    m_CurrentUsedBytes += NeededBytes;
    return SResult_OK;
}
} // Synthe