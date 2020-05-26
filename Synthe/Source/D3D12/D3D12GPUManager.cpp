// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GPUManager.hpp"
#include "D3D12Buffers.hpp"
#include "Common/Memory/Allocator.hpp"

#include <vector>

namespace Synthe {


DescriptorPool PSamplerDescriptorPools;
DescriptorPool PRtvDescriptorPools;
DescriptorPool PDsvDescriptorPools;
DescriptorPool PCbvSrvUavDescriptorPools;

// Our heaps are massive, but we can use them once.
MemoryPool PSamplerPool;
MemoryPool PRtvDsvPool;
MemoryPool PCbvSrvUavPool;
MemoryPool PScenePool;
MemoryPool PScratchPool;


GResult MemoryPool::Create(ID3D12Device* PDevice, Allocator* PAllocator, D3D12_HEAP_DESC& Desc, U64 SizeInBytes)
{
    m_Allocator = PAllocator;
    m_TotalSizeInBytes = 0;
    m_CurrentAllocatedBytes = 0;
    HRESULT Result = PDevice->CreateHeap(&Desc, __uuidof(ID3D12Heap), (void**)&m_Heap);
    if (FAILED(Result))
    {
        return GResult_CREATION_FAILURE;
    }
    return GResult_OK;
}


GResult MemoryPool::Release()
{
    if (m_Heap)
        m_Heap->Release();
    m_Heap = nullptr;
    m_TotalSizeInBytes = 0;
    m_CurrentAllocatedBytes = 0;
    return GResult_OK;
}


GResult MemoryPool::AllocateResource(ID3D12Device* PDevice, 
                                     D3D12_RESOURCE_DESC& Desc,
                                     D3D12_RESOURCE_STATES InitialState, 
                                     const D3D12_CLEAR_VALUE* ClearValue,
                                     ID3D12Resource** PPResource)
{
    // Allocate based on Total Size requested in resource description info.
    U64 TotalDimensionSize = Desc.Width * Desc.Height * Desc.DepthOrArraySize * Desc.MipLevels;
    U64 TotalSizeInBytes = 0ULL;
    U64 FormatChannelAlignmentBytes = 0;
    switch (Desc.Format)
    {
        case DXGI_FORMAT_UNKNOWN: 
        case DXGI_FORMAT_R8G8B8A8_UNORM: FormatChannelAlignmentBytes = 4ULL; break;
        case DXGI_FORMAT_R16G16B16A16_FLOAT: FormatChannelAlignmentBytes = 8ULL; break;
    }

    if (m_Allocator)
    {
        AllocationBlock Block = { };
        if (!m_Allocator->Allocate(&Block, TotalSizeInBytes, FormatChannelAlignmentBytes))
        {
            return GResult_MEMORY_ALLOCATION_FAILURE;
        }
        PDevice->CreatePlacedResource(m_Heap, 
                                      Block.Offset, 
                                      &Desc, 
                                      InitialState, 
                                      ClearValue, 
                                      __uuidof(ID3D12Resource), (void**)PPResource);
        m_AllocatedBlocks[*PPResource] = Block;
    }
    else
    {
        return GResult_REFUSE_CALL;
    }
    return GResult_OK;
}


DescriptorPool::DescriptorPool()
    : m_DescriptorHeap(nullptr)
    , m_LastCpuHandle({0})
    , m_LastGpuHandle({0})
{
}


DescriptorPool::~DescriptorPool()
{
    Release();
}


GResult DescriptorPool::Create(ID3D12Device* PDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS Flags, U32 NumDescriptors)
{
    D3D12_DESCRIPTOR_HEAP_DESC Desc = { };
    Desc.Flags = Flags;
    Desc.NodeMask = 0;
    Desc.NumDescriptors = NumDescriptors;
    Desc.Type = Type;
    HRESULT Result = PDevice->CreateDescriptorHeap(&Desc, __uuidof(ID3D12DescriptorHeap), (void**)&m_DescriptorHeap);
    if(FAILED(Result))
    {
        return GResult_CREATION_FAILURE;
    }
    m_LastCpuHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_LastGpuHandle = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
    m_AlignmentSize = PDevice->GetDescriptorHandleIncrementSize(Type);
    return GResult_OK;
}


GResult DescriptorPool::Release()
{
    if (m_DescriptorHeap)
        m_DescriptorHeap->Release();
    m_DescriptorHeap = nullptr;
    return GResult_OK;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateSrv(ID3D12Device* PDevice, D3D12_SHADER_RESOURCE_VIEW_DESC& Info, ID3D12Resource* PResource)
{
    PDevice->CreateShaderResourceView(PResource, &Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateDsv(ID3D12Device* PDevice, D3D12_DEPTH_STENCIL_VIEW_DESC& Info, ID3D12Resource* PResource)
{
    PDevice->CreateDepthStencilView(PResource, &Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateRtv(ID3D12Device* PDevice, D3D12_RENDER_TARGET_VIEW_DESC& Info, ID3D12Resource* PResource)
{
    PDevice->CreateRenderTargetView(PResource, &Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateCbv(ID3D12Device* PDevice, D3D12_CONSTANT_BUFFER_VIEW_DESC& Info)
{
    PDevice->CreateConstantBufferView(&Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateUav(ID3D12Device* PDevice, D3D12_UNORDERED_ACCESS_VIEW_DESC& Info, ID3D12Resource* PCounterResource, ID3D12Resource* PResource)
{
    PDevice->CreateUnorderedAccessView(PResource, PCounterResource, &Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateSampler(ID3D12Device* PDevice, D3D12_SAMPLER_DESC& Info)
{
    PDevice->CreateSampler(&Info, m_LastCpuHandle);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_LastCpuHandle;
    m_LastCpuHandle.ptr += m_AlignmentSize;
    return Handle;
}


void D3D12GPUMemoryManager::InitializeRtvDsvHeap(ID3D12Device* PDevice, U64 SizeInBytes)
{
    //PRtvDsvPool.Create(PDevice, )
}
} // Synthe 