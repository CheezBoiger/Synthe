// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#include "Common/Memory/FreeListAllocator.hpp"

namespace Synthe {


ResultCode FreeListAllocator::Allocate(AllocationBlock* Block, U64 SizeInBytes, U64 Alignment)
{
    FreeListBlock* Header = m_Root;
    
    while (Header)
    {
        if (!Header->PNext)
        {
            // Reinterpret the new block from the end of the last memory block.
            FreeListBlock* NewBlock = 
                reinterpret_cast<FreeListBlock*>(Header->StartingAddress + Header->SizeInBytes);
            NewBlock->ShouldFree = false;
            NewBlock->SizeInBytes = ALIGN_BYTES(SizeInBytes, Alignment);

            // Actual memory of the object allocated starts after the FreeListBlock header.
            NewBlock->StartingAddress = reinterpret_cast<U64>(NewBlock) + sizeof(FreeListBlock);

            // Check if the new block will step out of bounds of the avaiable pool memory.
            if ((NewBlock->StartingAddress + NewBlock->SizeInBytes) >= m_TotalSizeInBytes)
            {
                return SResult_OUT_OF_MEMORY;
            }

            // Reassign the previous block next with the newly allocated block.
            Header->PNext = NewBlock;
            return SResult_OK;
        }
        else
        {
            // If the current header is set to be freed, we can use it!
            if (Header->ShouldFree)
            {
                // Check if the mmeory block is big enough.
                // Check if we should break up the memory block.
                // Check if the memory block is exactly what we need!
            }

            
            Header = Header->PNext;
        }
    }
    return SResult_NOT_IMPLEMENTED;
}


ResultCode FreeListAllocator::Free(AllocationBlock* Block)
{
    return SResult_NOT_IMPLEMENTED;
}


void FreeListAllocator::Reset()
{
}


void FreeListAllocator::OnInitialize()
{
}
} // Synthe