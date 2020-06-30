// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12DescriptorManager.hpp"
#include "D3D12MemoryManager.hpp"

#include "Common/Memory/LinearAllocator.hpp"
#include "Common/Memory/NewAllocator.hpp"


namespace Synthe {


std::unordered_map<D3D12MemoryManager::MemoryKeyID, MemoryPool> MemoryPoolCache;
std::unordered_map<D3D12MemoryManager::MemoryKeyID, Allocator*> AllocatorPoolCache;
std::unordered_map<GPUHandle, ResourceState> ResourceCache;

U64 D3D12MemoryManager::k_TotalGPUMemoryBytes = 0ULL;
U64 D3D12MemoryManager::k_TotalCPUMemoryBytes = 0ULL;


ResultCode D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryKeyID Key)
{
    MemoryPoolCache[Key] = MemoryPool();
    return SResult_OK;
}


MemoryPool* D3D12MemoryManager::GetMemoryPool(MemoryKeyID Key)
{
    if (MemoryPoolCache.find(Key) != MemoryPoolCache.end())
    {
        return &MemoryPoolCache[Key];
    }
    return nullptr;
}


ResultCode D3D12MemoryManager::DestroyMemoryPoolsAtKey(MemoryKeyID Key)
{
    if (MemoryPoolCache.find(Key) != MemoryPoolCache.end())
    {
        MemoryPoolCache[Key].Release();
        MemoryPoolCache.erase(Key);
        return SResult_OK;
    }
    return SResult_OBJECT_NOT_FOUND;
}


ResultCode MemoryPool::Create(ID3D12Device* PDevice, Allocator* PAllocator, D3D12_HEAP_DESC& Desc)
{
    m_Allocator = PAllocator;
    m_TotalSizeInBytes = Desc.SizeInBytes;
    m_CurrentAllocatedBytes = 0ULL;
    HRESULT Result = PDevice->CreateHeap(&Desc, __uuidof(ID3D12Heap), (void**)&m_Heap);
    if (FAILED(Result))
    {
        return GResult_DEVICE_CREATION_FAILURE;
    }
    
    if (Desc.Properties.Type == D3D12_HEAP_TYPE_DEFAULT) 
    {
        D3D12MemoryManager::k_TotalGPUMemoryBytes += Desc.SizeInBytes;
    } 
    else 
    {
        D3D12MemoryManager::k_TotalCPUMemoryBytes += Desc.SizeInBytes;
    }
   
    return SResult_OK;
}


ResultCode MemoryPool::Release()
{
    if (m_Heap)
    {
        m_Heap->Release();
    }
    m_Heap = nullptr;
    m_TotalSizeInBytes = 0;
    m_CurrentAllocatedBytes = 0;
    return SResult_OK;
}


ResultCode MemoryPool::AllocateResource(ID3D12Device* PDevice, 
                                        D3D12_RESOURCE_DESC& Desc,
                                        D3D12_RESOURCE_STATES InitialState, 
                                        const D3D12_CLEAR_VALUE* ClearValue,
                                        ID3D12Resource** PPResource)
{
    D3D12_RESOURCE_ALLOCATION_INFO AllocationInfo = { 0ULL, 0ULL };
    D3D12MemoryManager::GetCachedResourceSize(PDevice, Desc, &AllocationInfo);

    if (m_Allocator)
    {
        AllocationBlock Block = { };
        if (!m_Allocator->Allocate(&Block, AllocationInfo.SizeInBytes, AllocationInfo.Alignment))
        {
            return SResult_MEMORY_ALLOCATION_FAILURE;
        }
        HRESULT Result = PDevice->CreatePlacedResource(m_Heap, Block.StartAddress, 
            &Desc, InitialState, ClearValue, __uuidof(ID3D12Resource), (void**)PPResource);
        m_AllocatedBlocks[*PPResource] = Block;
    }
    else
    {
        return SResult_REFUSE_CALL;
    }
    return SResult_OK;
}


ResultCode MemoryPool::FreeResource(ID3D12Resource* PResource)
{
    if (!m_Allocator)
    {
        return SResult_INITIALIZATION_FAILURE;
    }
    AllocationBlock Block = m_AllocatedBlocks[PResource];
    if (m_Allocator->Free(&Block))
    {
        m_AllocatedBlocks.erase(PResource);
    } 
    else 
    {
        return SResult_FAILED;
    }
    return SResult_OK;
}


void D3D12MemoryManager::CreateAndRegisterAllocator(MemoryKeyID Key, 
                                                    AllocT AllocatorType, 
                                                    Allocator* PAllocator)
{
    if (AllocatorPoolCache.find(Key) == AllocatorPoolCache.end())
    {
        switch (AllocatorType)
        {
            case AllocType_CUSTOM:
                AllocatorPoolCache[Key] = PAllocator;
                break;
            case AllocType_LINEAR:
                AllocatorPoolCache[Key] = Malloc<LinearAllocator>();
                break;
            case AllocType_NEW:
            default:
                AllocatorPoolCache[Key] = Malloc<NewAllocator>();
        }
    }
}


Allocator* D3D12MemoryManager::GetAllocator(MemoryKeyID Key)
{
    Allocator* PAllocator = nullptr;
    if (AllocatorPoolCache.find(Key) != AllocatorPoolCache.end())
    {
        PAllocator = AllocatorPoolCache[Key];
    }
    return PAllocator;
}


ResultCode D3D12MemoryManager::DestroyAllocatorAtKey(MemoryKeyID Key)
{
    if (AllocatorPoolCache.find(Key) != AllocatorPoolCache.end())
    {
        Free(AllocatorPoolCache[Key]);
        AllocatorPoolCache.erase(Key);
        return SResult_OK;
    }
    return SResult_OBJECT_NOT_FOUND;
}


ResultCode D3D12MemoryManager::CacheNativeResource(GPUHandle Key, ID3D12Resource* PResource, D3D12_RESOURCE_STATES InitialState)
{
    if (ResourceCache.find(Key) != ResourceCache.end())
    {
        return SResult_ALREADY_EXISTS;
    }
    ResourceCache[Key] = { PResource, InitialState };
    return SResult_OK;
}


ResultCode D3D12MemoryManager::GetNativeResource(GPUHandle Key, ResourceState* POutResource)
{
    if (ResourceCache.find(Key) == ResourceCache.end())
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    *POutResource = ResourceCache[Key];
    return SResult_OK;
}


ResultCode D3D12MemoryManager::UpdateResourceState(GPUHandle Key, D3D12_RESOURCE_STATES State)
{
    if (ResourceCache.find(Key) == ResourceCache.end())
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    ResourceCache[Key].State = State;
    return SResult_OK;
}


ResultCode D3D12MemoryManager::GetCachedResourceSize(ID3D12Device* PDevice, 
                                                     D3D12_RESOURCE_DESC& Desc,
                                                     D3D12_RESOURCE_ALLOCATION_INFO* Out)
{
    // Allocate based on Total Size requested in resource description info.
    U64 TotalDimensionSize = Desc.Width * Desc.Height * Desc.DepthOrArraySize * Desc.MipLevels;
    // We also need our pixel depth/width of the format it will be created with.
    U64 FormatSizeInBytes = static_cast<U64>(GetBitsForPixelFormat(Desc.Format));
    U64 SampleCount = Desc.SampleDesc.Count;
    U64 TotalSizeInBytes = TotalDimensionSize * FormatSizeInBytes * SampleCount;
    Out->SizeInBytes = TotalSizeInBytes;

    // Alignments on devices are constant in D3D12, which is quite nice. 
    // In Vulkan, this wouldn't be the case, and we would require querying from 
    // vkGetBufferMemoryRequirements/vkGetImageMemoryRequirements.
    switch (Desc.Dimension)
    {
        case D3D12_RESOURCE_DIMENSION_BUFFER:
            Out->Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            break;
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
        case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
        {
            if (Desc.SampleDesc.Count > 1)
            {
                if (TotalSizeInBytes <= D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT)
                {
                    Out->Alignment = D3D12_SMALL_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
                }
                else
                {
                    Out->Alignment = D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
                }
            }
            else
            {
                if (TotalSizeInBytes <= D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)
                {
                    Out->Alignment = D3D12_SMALL_RESOURCE_PLACEMENT_ALIGNMENT;
                } 
                else 
                {
                    Out->Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
                }
            }
            break;
        }
    }
    return SResult_OK;
}


ResultCode D3D12MemoryManager::RemoveCachedNatvieResource(GPUHandle Key)
{
    if (ResourceCache.find(Key) == ResourceCache.end())
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    ResourceCache.erase(Key);
    return SResult_OK;
}
} // Synthe