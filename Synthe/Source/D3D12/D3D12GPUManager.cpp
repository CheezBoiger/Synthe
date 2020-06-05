// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GPUManager.hpp"
#include "D3D12Buffers.hpp"
#include "Common/Memory/Allocator.hpp"

#include <vector>

namespace Synthe {


DescriptorPool PRtvDescriptorPools;
DescriptorPool PDsvDescriptorPools;

std::vector<DescriptorPool> PCbvSrvUavUploadDescriptorPools;
std::vector<DescriptorPool> PSamplerUploadDescriptorPools;

DescriptorPool GPUCbvSrvUavDescriptorHeap;
DescriptorPool GPUSamplerDescriptorHeap;

// Our heaps are massive, but we can use them once.
MemoryPool PSamplerPool;
MemoryPool PRtvDsvPool;
MemoryPool PCbvSrvUavPool;
MemoryPool PScenePool;
MemoryPool PScratchPool;


DescriptorPool& D3D12GPUMemoryManager::DescriptorPoolRtv()
{
    return PRtvDescriptorPools;
}


DescriptorPool& D3D12GPUMemoryManager::DescriptorPoolDsv()
{
    return PDsvDescriptorPools;
}


void D3D12GPUMemoryManager::InitializeGPUCbvSrvUavDescriptorHeap(ID3D12Device* PDevice, U32 NumDescriptors)
{
    GResult Result = GPUCbvSrvUavDescriptorHeap.Create(PDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                                                       D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, NumDescriptors);
    if (Result != GResult_OK)
    {
        return;
    }
}


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
    m_Allocator->Initialize(0, SizeInBytes);
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
    // We also need our pixel depth/width of the format it will be created with.
    U64 FormatSizeInBytes = static_cast<U64>(GetBitsForPixelFormat(Desc.Format));
    U64 TotalSizeInBytes = TotalDimensionSize * FormatSizeInBytes;
    static const U64 FormatChannelAlignmentBytes = 4ULL;

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


GResult MemoryPool::FreeResource(ID3D12Resource* PResource)
{
    
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
    m_AlignmentSizeInBytes = PDevice->GetDescriptorHandleIncrementSize(Type);
    m_TotalDescriptorHeapSizeInBytes = m_AlignmentSizeInBytes * NumDescriptors;
    m_DescriptorHeapType = Type;
    return GResult_OK;
}


D3D12_GPU_DESCRIPTOR_HANDLE DescriptorPool::GetGPUAddressFromCPUAddress(D3D12_CPU_DESCRIPTOR_HANDLE Handle)
{
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle = GetBaseCPUAddress();
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle = GetBaseGPUAddress();
    if ((Handle.ptr - CPUHandle.ptr) > m_TotalDescriptorHeapSizeInBytes)
    {
        return GPUHandle;
    }
    GPUHandle.ptr += (Handle.ptr - CPUHandle.ptr);
    return GPUHandle;
}


GResult DescriptorPool::Release()
{
    if (m_DescriptorHeap)
        m_DescriptorHeap->Release();
    m_DescriptorHeap = nullptr;
    return GResult_OK;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateSrv(ID3D12Device* PDevice, 
                                                      D3D12_SHADER_RESOURCE_VIEW_DESC& Info, 
                                                      ID3D12Resource* PResource,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateShaderResourceView(PResource, &Info, Handle);
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateDsv(ID3D12Device* PDevice, 
                                                      D3D12_DEPTH_STENCIL_VIEW_DESC& Info, 
                                                      ID3D12Resource* PResource,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateDepthStencilView(PResource, &Info, Handle);
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateRtv(ID3D12Device* PDevice, 
                                                      D3D12_RENDER_TARGET_VIEW_DESC& Info, 
                                                      ID3D12Resource* PResource,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateRenderTargetView(PResource, &Info, Handle);
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateCbv(ID3D12Device* PDevice, 
                                                      D3D12_CONSTANT_BUFFER_VIEW_DESC& Info,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateConstantBufferView(&Info, Handle);
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateUav(ID3D12Device* PDevice, 
                                                      D3D12_UNORDERED_ACCESS_VIEW_DESC& Info, 
                                                      ID3D12Resource* PCounterResource, 
                                                      ID3D12Resource* PResource,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateUnorderedAccessView(PResource, PCounterResource, &Info, Handle);
    return Handle;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPool::CreateSampler(ID3D12Device* PDevice, 
                                                          D3D12_SAMPLER_DESC& Info,
                                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = LocationInDescriptorHeap;
    if (Handle.ptr == ADDRESS_SZ_MAX) 
    {
        Handle = m_LastCpuHandle;
        m_LastCpuHandle.ptr += m_AlignmentSizeInBytes;
    }
    PDevice->CreateSampler(&Info, Handle);
    return Handle;
}


// Initialize Descriptor Table.
void InitializeDescriptorTable(D3D12_GPU_DESCRIPTOR_HANDLE DescriptorPoolBaseGPUAddress, 
                               U64 OffsetInBytes, 
                               DescriptorTable* POutTable)
{
    DescriptorPoolBaseGPUAddress.ptr += OffsetInBytes;
    POutTable->TableSizeInBytes = OffsetInBytes;
    POutTable->StartingAddress = DescriptorPoolBaseGPUAddress.ptr;  
}


GResult DescriptorPool::CopyDescriptorsRange(ID3D12Device* PDevice,
                                             U32 NumSrcDescriptors,
                                             D3D12_CPU_DESCRIPTOR_HANDLE* PSrcDescriptorHandles,
                                             U64 OffsetInDescriptorCount,
                                             DescriptorTable* POutTable)
{ 
    for (U32 I = 0; I < NumSrcDescriptors; ++I)     
    {
        // Each individual descriptor handle is copied, this is not the best efficiency, but 
        // if each descriptor handle is not consecutive, we will need to copy individually.
        CopyDescriptorsRangeConsecutive(PDevice, PSrcDescriptorHandles[I], 1, 
                                        static_cast<U32>(OffsetInDescriptorCount) + I, nullptr);
    }

    if (POutTable) 
    {
        U64 OffsetInBytes = OffsetInDescriptorCount * m_AlignmentSizeInBytes;
        InitializeDescriptorTable(GetBaseGPUAddress(), OffsetInBytes, POutTable);
    } 
    else 
    {
        return GResult_MEMORY_NULL_EXCEPTION;
    }
    return GResult_OK;
}


GResult DescriptorPool::CopyDescriptorsRangeConsecutive(ID3D12Device* PDevice,
                                                        D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorHandle,
                                                        U32 SrcDescriptorSize,
                                                        U32 DstOffsetDescriptorCount,
                                                        DescriptorTable* POutTable)
{
    U64 OffsetInBytes = static_cast<U64>(DstOffsetDescriptorCount * m_AlignmentSizeInBytes);
    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableOffsetAddress = GetBaseCPUAddress();
    DescriptorTableOffsetAddress.ptr += OffsetInBytes;
    PDevice->CopyDescriptorsSimple(SrcDescriptorSize, DescriptorTableOffsetAddress, SrcDescriptorHandle, m_DescriptorHeapType);
    if (POutTable)
    {
        InitializeDescriptorTable(GetBaseGPUAddress(), OffsetInBytes, POutTable);
    } 
    else 
    {
        return GResult_MEMORY_NULL_EXCEPTION;
    }
    return GResult_OK;
}


GResult DescriptorPool::CopyDescriptorRanges(ID3D12Device* PDevice,
                                             U32 NumSrcDescriptorStarts,
                                             D3D12_CPU_DESCRIPTOR_HANDLE* PSrcDescriptorHandleStarts,
                                             U32* PSrcDescriptorHandleCounts,
                                             U32* NumDstOffsetsInDescriptorCount,
                                             U32 NumDescriptorTableOuts,
                                             DescriptorTable** PPDescriptorTables)
{
    static D3D12_CPU_DESCRIPTOR_HANDLE DstCPUHandles[128];
    static U32 DstCPUSizes[128];
    static U32 SrcCPUSizes[128];
    U32 NumDstDescriptorRanges = 0;

    D3D12_CPU_DESCRIPTOR_HANDLE BasePoolAddress = GetBaseCPUAddress();
    for (U32 I = 0; I < NumSrcDescriptorStarts; ++I)
    {
        U64 SizeInBytes = static_cast<U64>(NumDstOffsetsInDescriptorCount[I] * m_AlignmentSizeInBytes);
        D3D12_GPU_DESCRIPTOR_HANDLE PoolGPUAddressOffset = { GetBaseGPUAddress().ptr + NumDstOffsetsInDescriptorCount[I] };
        DstCPUHandles[I].ptr = BasePoolAddress.ptr + SizeInBytes;
        DstCPUSizes[I] = static_cast<U32>(SizeInBytes);

        if (PPDescriptorTables[I])
        {
            PPDescriptorTables[I]->StartingAddress = PoolGPUAddressOffset.ptr;
            PPDescriptorTables[I]->TableSizeInBytes = SizeInBytes;
        }
    }
    
    PDevice->CopyDescriptors(NumDstDescriptorRanges, 
                             DstCPUHandles, 
                             DstCPUSizes, 
                             NumSrcDescriptorStarts, 
                             PSrcDescriptorHandleStarts, 
                             SrcCPUSizes,     
                             m_DescriptorHeapType);
    
    return GResult_OK;
}
} // Synthe 