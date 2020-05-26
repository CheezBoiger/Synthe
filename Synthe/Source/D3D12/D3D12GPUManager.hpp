// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once 

#include "Win32Common.hpp"
#include "Common/Memory/Allocator.hpp"

#include <vector>
#include <unordered_map>

namespace Synthe {


class Allocator;


class MemoryPool
{
public:
    GResult AllocateResource(ID3D12Device* PDevice,
                             D3D12_RESOURCE_DESC& Desc, 
                             D3D12_RESOURCE_STATES InitialState,
                             const D3D12_CLEAR_VALUE* ClearValue,
                             ID3D12Resource** PResource);
    GResult Create(ID3D12Device* PDevice, 
                   Allocator* Allocator,
                   D3D12_HEAP_DESC& Desc, 
                   U64 SizeInBytes);

    GResult FreeResource(ID3D12Resource* PResource);
    GResult Release();

private:
    ID3D12Heap* m_Heap;
    Allocator* m_Allocator;

    U64 m_TotalSizeInBytes;
    U64 m_CurrentAllocatedBytes;
    // Map containing resources and their allocation info counterpart.
    std::unordered_map<ID3D12Resource*, AllocationBlock> m_AllocatedBlocks;
};


class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    GResult Create(ID3D12Device* PDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS Flags, U32 NumDescriptors);
    GResult Release();
    
    D3D12_CPU_DESCRIPTOR_HANDLE CreateSrv(ID3D12Device* PDevice, 
                                          D3D12_SHADER_RESOURCE_VIEW_DESC& Info, 
                                          ID3D12Resource* PResource);
    D3D12_CPU_DESCRIPTOR_HANDLE CreateDsv(ID3D12Device* PDevice,
                                          D3D12_DEPTH_STENCIL_VIEW_DESC& Info,
                                          ID3D12Resource* PResource);
    D3D12_CPU_DESCRIPTOR_HANDLE CreateRtv(ID3D12Device* PDevice,
                                          D3D12_RENDER_TARGET_VIEW_DESC& Info,
                                          ID3D12Resource* PResource);
    D3D12_CPU_DESCRIPTOR_HANDLE CreateCbv(ID3D12Device* PDevice,
                                          D3D12_CONSTANT_BUFFER_VIEW_DESC& Info);
    D3D12_CPU_DESCRIPTOR_HANDLE CreateUav(ID3D12Device* PDevice,
                                          D3D12_UNORDERED_ACCESS_VIEW_DESC& Info,
                                          ID3D12Resource* PCounterResource,
                                          ID3D12Resource* PResource);
    D3D12_CPU_DESCRIPTOR_HANDLE CreateSampler(ID3D12Device* PDevice,
                                              D3D12_SAMPLER_DESC& Info);
    //D3D12_DESCRIPTOR_HEAP_TYPE GetDescriptorHeapType() {}

    D3D12_CPU_DESCRIPTOR_HANDLE GetBaseCpuAddress() const { return m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(); }
    D3D12_GPU_DESCRIPTOR_HANDLE GetBaseGpuAddress() const { return m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(); }

private:
    ID3D12DescriptorHeap* m_DescriptorHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_LastCpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_LastGpuHandle;
    UINT m_AlignmentSize;
};


class DescriptorTable
{
public:
    void Initialize(DescriptorPool& Pool, D3D12_CPU_DESCRIPTOR_HANDLE* PDescriptorHandles, U32 DescriptorCount);

    D3D12_GPU_VIRTUAL_ADDRESS GetTableBaseAddress() { return m_StartingAddress; }

private:
    D3D12_GPU_VIRTUAL_ADDRESS m_StartingAddress;
};


class D3D12GPUMemoryManager {
public:
    void InitializeSamplerPool(ID3D12Device* PDevice, U64 SizeInBytes);
    void InitializeRtvDsvHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    void InitializeCbvSrvUavHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    void InitializeSceneMemoryHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    void InitializeScratchMemoryHeap(ID3D12Device* PDevice, U64 SizeInBytes);    

    static MemoryPool& GetSamplerHeap();
    static MemoryPool& GetRtvDsvHeap();
    static MemoryPool& GetCbvSrvUavHeap();

    static MemoryPool& GetSceneMemoryHeap();
    static MemoryPool& GetScratchMemoryHeap();
};
} // Synthe 