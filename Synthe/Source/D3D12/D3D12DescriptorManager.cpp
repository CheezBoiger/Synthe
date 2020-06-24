// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12DescriptorManager.hpp"
#include "D3D12Buffers.hpp"
#include "Common/Memory/Allocator.hpp"

#include <vector>
#include <unordered_map>

namespace Synthe {

std::unordered_map<DescriptorKeyID, std::vector<DescriptorPool>> DescriptorPoolCache; 


ResultCode D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorKeyID Key, U32 NumPools)
{
    DescriptorPoolCache[Key].resize(NumPools);
    for (U32 I = 0; I < NumPools; ++I)
    {
        DescriptorPoolCache[Key][I] = DescriptorPool();
    }
    return SResult_OK;
}


DescriptorPool* D3D12DescriptorManager::GetDescriptorPool(DescriptorKeyID Key, U32 Index)
{
    if (DescriptorPoolCache.find(Key) != DescriptorPoolCache.end())
    {
        return &DescriptorPoolCache[Key][Index];
    }
    return nullptr;
}


ResultCode D3D12DescriptorManager::DestroyDescriptorPoolsAtKey(DescriptorKeyID Key)
{
    if (DescriptorPoolCache.find(Key) != DescriptorPoolCache.end())
    {
        for (U32 I = 0; I < DescriptorPoolCache[Key].size(); ++I)
        {
            DescriptorPoolCache[Key][I].Release();
        }
    }
    DescriptorPoolCache[Key].clear();
    return SResult_OK;
}


DescriptorPool::DescriptorPool()
    : m_DescriptorHeap(nullptr)
    , m_LastCpuHandle({0})
    , m_LastGpuHandle({0})
    , m_AlignmentSizeInBytes(0)
    , m_DescriptorHeapType(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
    , m_TotalDescriptorHeapSizeInBytes(0)
{
}


DescriptorPool::~DescriptorPool()
{
    Release();
}


ResultCode DescriptorPool::Create(ID3D12Device* PDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS Flags, U32 NumDescriptors)
{
    D3D12_DESCRIPTOR_HEAP_DESC Desc = { };
    Desc.Flags = Flags;
    Desc.NodeMask = 0;
    Desc.NumDescriptors = NumDescriptors;
    Desc.Type = Type;
    HRESULT Result = PDevice->CreateDescriptorHeap(&Desc, __uuidof(ID3D12DescriptorHeap), (void**)&m_DescriptorHeap);
    if(FAILED(Result))
    {
        return GResult_DEVICE_CREATION_FAILURE;
    }
    m_LastCpuHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_LastGpuHandle = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
    m_AlignmentSizeInBytes = PDevice->GetDescriptorHandleIncrementSize(Type);
    m_TotalDescriptorHeapSizeInBytes = m_AlignmentSizeInBytes * NumDescriptors;
    m_DescriptorHeapType = Type;
    return SResult_OK;
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


ResultCode DescriptorPool::Release()
{
    if (m_DescriptorHeap)
        m_DescriptorHeap->Release();
    m_DescriptorHeap = nullptr;
    return SResult_OK;
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


ResultCode DescriptorPool::CopyDescriptorsRange(ID3D12Device* PDevice,
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
        return SResult_MEMORY_NULL_EXCEPTION;
    }
    return SResult_OK;
}


ResultCode DescriptorPool::CopyDescriptorsRangeConsecutive(ID3D12Device* PDevice,
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
        return SResult_MEMORY_NULL_EXCEPTION;
    }
    return SResult_OK;
}


ResultCode DescriptorPool::CopyDescriptorRanges(ID3D12Device* PDevice,
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
    
    return SResult_OK;
}
} // Synthe 