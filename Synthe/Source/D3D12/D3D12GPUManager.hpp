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

//! MemoryPool is the handler for internal API objects. This is in terms of the 
//! D3D12 Heap handler. Resources are created via this object, should they need
//! to be allocated within user managed pools.
class MemoryPool
{
public:
    //! The allocation call function, used for creating the resource
    //! object, along with the raw memory handle.
    //! \param PDevice The device handle from the Application context.
    //! \param Desc The description of the resource to create.
    //! \param InitialState The initial resource state that the resource will be created
    //!                     with. This is the starting state, and must be transitioned if 
    //!                     it's handling requires so.
    //! \param ClearValue Optimized clear value that will be created with the resource,
    //!                   this is a good value to use that will let the hardware optimize clearing
    //!                   calls for this resource.
    //! \param PResource The output resource handle that will store the newly created resource.
    //! \return The resulting code. GResult_OK is returned if succeeded, along with the PResource output,
    //!         otherwise will return another error code if the call fails.
    GResult AllocateResource(ID3D12Device* PDevice,
                             D3D12_RESOURCE_DESC& Desc, 
                             D3D12_RESOURCE_STATES InitialState,
                             const D3D12_CLEAR_VALUE* ClearValue,
                             ID3D12Resource** PResource);

    //! Create the Memory Pool Native Handle. This is internally handled,
    //! Whereas the Allocator is the method used for handling.
    //! 
    //! \param PDevice The device handle from the Application context.
    //! \param Allocator The data structure used for handling the internal native 
    //!                  API object.
    //! \param Desc The description type of the heap.
    //! \param SizeInBytes The size of the entire memory pool.
    //! \return The resulting code OK, if the 
    GResult Create(ID3D12Device* PDevice, 
                   Allocator* Allocator,
                   D3D12_HEAP_DESC& Desc, 
                   U64 SizeInBytes);

    //! Free the resource that was allocated with this MemoryPool.
    //!
    //! \param PResource The Resource that will be freed.
    //! \return The resulting code. GResult_OK if the call succeeds, and the resource is 
    //!         invalid. Other code retured if failed.
    GResult FreeResource(ID3D12Resource* PResource);

    //! Release the MemoryPool's API handle. This is a clean up function.
    //! \return The resulting code. GResult_OK if the call successfully releases the internal
    //!         handle. Other code if the cleanup fails.
    GResult Release();

private:
    //! The Internal API heap handle. This is a D3D12 handle created by the Create() member.
    //! \sa Create()
    ID3D12Heap* m_Heap;

    //! The allocator data structure, used for managing allocations from this memory pool.
    //! \sa Create()
    Allocator* m_Allocator;

    //! The total size of the memory pool, in bytes.
    U64 m_TotalSizeInBytes;

    //! The current size of all allocations done in this memory pool.
    U64 m_CurrentAllocatedBytes;

    //! Map containing resources and their allocation info counterpart.
    std::unordered_map<ID3D12Resource*, AllocationBlock> m_AllocatedBlocks;
};


//! Descriptor table is an object that holds the handle to a base address of a descriptor heap,
//! along with the size of the table. These are used to represent arrays of descriptors to be 
//! bound on render command recording.
struct DescriptorTable
{
    //! The starting GPU address of the descriptor table.
    D3D12_GPU_VIRTUAL_ADDRESS StartingAddress;
    
    //! The end GPU address of the descriptor table.
    UINT64 TableSizeInBytes;
};


//! Descriptor pool handles the descriptor heaps of D3D12, which will be used for storing
//! descriptors, as well as handling the creation of views and samplers. This is a dynamic
//! descriptor manager, so it is active when copies are being done.
class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    //! Create the descriptor heap, along with the number of descriptors for the pool.
    //!
    //! \param PDevice The device from the application context.
    //! \param Type The type of descriptor heap to be created.
    //! \param Flags Additional flags that will be created for this descriptor heap pool.
    //! \param NumDescriptors The number of descriptors that will be made available to the 
    //!                       descriptor heap pool.
    //! \return The resulting code. GResult_OK if the creation of the descriptor heap pool succeeds.
    //!         Otherwise, another code will result if the call fails.
    GResult Create(ID3D12Device* PDevice, 
                   D3D12_DESCRIPTOR_HEAP_TYPE Type, 
                   D3D12_DESCRIPTOR_HEAP_FLAGS Flags, 
                   U32 NumDescriptors);

    //! Releases the descriptor pool handle of the D3D12 Descriptor heap.
    //!
    //! \return The resulting code. GResult_OK if descriptor pool was successfully cleaned up.
    //!         Any other code otherwise should signal a failure.
    GResult Release();
    
    //! Create a Shader Resource View from this descriptor pool.
    //!
    //! \param PDevice
    //! \param Info
    //! \param PResource
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the shader resource view. Null descriptor handle is 
    //!         returned if the creation has failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateSrv(ID3D12Device* PDevice, 
                                          D3D12_SHADER_RESOURCE_VIEW_DESC& Info, 
                                          ID3D12Resource* PResource,
                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);
    
    //! Create a Depth Stencil View from this descriptor pool.
    //!
    //! \param PDevice
    //! \param Info
    //! \param PResource
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the depth stencil view. Null descriptor handle is 
    //!         returned if the creation has failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateDsv(ID3D12Device* PDevice,
                                          D3D12_DEPTH_STENCIL_VIEW_DESC& Info,
                                          ID3D12Resource* PResource,
                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);

    //! Create a Render Target View from this descriptor pool.
    //! 
    //! \param PDevice
    //! \param Info
    //! \param PResource
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the render target view. Null descriptor handle is returned
    //!         if the creation has failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateRtv(ID3D12Device* PDevice,
                                          D3D12_RENDER_TARGET_VIEW_DESC& Info,
                                          ID3D12Resource* PResource, 
                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);

    //! Create a Constant Buffer View from this descriptor pool.
    //!
    //! \param PDevice
    //! \param Info
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the constant buffer view. Null descriptor handle is returned
    //!         if the creation has failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateCbv(ID3D12Device* PDevice,
                                          D3D12_CONSTANT_BUFFER_VIEW_DESC& Info,
                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);

    //! Create an Undordered Access View from this descriptor pool.
    //! 
    //! \param PDevice
    //! \param Info
    //! \param PCounterResource
    //! \param PResource
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the unordered access view. Null descriptor handle is returned
    //!         if the creation has failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateUav(ID3D12Device* PDevice,
                                          D3D12_UNORDERED_ACCESS_VIEW_DESC& Info,
                                          ID3D12Resource* PCounterResource,
                                          ID3D12Resource* PResource,
                                          D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);

    //! Create a Sampler from this descriptor pool.
    //!
    //! \param PDevice
    //! \param Info
    //! \param LocationInDescriptorHeap Optional location to create the descriptor in this descriptor pool.
    //!                                 If none is defined, it will immediately append to the last.
    //! \return The descriptor handle to the sampler. Null descriptor handle is returned if the creation is failed.
    D3D12_CPU_DESCRIPTOR_HANDLE CreateSampler(ID3D12Device* PDevice,
                                              D3D12_SAMPLER_DESC& Info,
                                              D3D12_CPU_DESCRIPTOR_HANDLE LocationInDescriptorHeap = BASE_CPU_DESCRIPTOR_ALLOC);
    
    //! Get The descriptor heap type. 
    //! \return The type of this descriptor pool.
    D3D12_DESCRIPTOR_HEAP_TYPE GetDescriptorHeapType() const { return m_DescriptorHeapType; }

    //! Copy descriptor handles in a single range that does not contain a consecutive list of cpu handles, 
    //! along with the cpu address offset in the pool. 
    //! 
    //! \param PDevice  
    //! \param NumSrcDescriptors
    //! \param PSrcDescriptorHandles
    //! \param OffsetInDescriptorCount
    //! \param POutTable
    //! \return A resulting code. GResult_OK should also return with data for the POutTable parameter.
    //!         Should any other result code be returned, then no data will be written onto the POutTable
    //!         parameter.
    GResult CopyDescriptorsRange(ID3D12Device* PDevice,
                                 U32 NumSrcDescriptors,
                                 D3D12_CPU_DESCRIPTOR_HANDLE* PSrcDescriptorHandles,
                                 U64 OffsetInDescriptorCount,
                                 DescriptorTable* POutTable);

    //! Copy a descriptor range that contains consecutive handles, so a single handle is required.
    //!
    //! \param PDevice
    //! \param PSrcDescriptorHandle
    //! \param SrcDescriptorSize
    //! \param DstOffsetDescriptorCount
    //! \return The resulting code.
    GResult CopyDescriptorsRangeConsecutive(ID3D12Device* PDevice,
                                            D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorHandle,
                                            U32 SrcDescriptorSize,
                                            U32 DstOffsetInDescriptorCount,
                                            DescriptorTable* POutTable);

    //! Copy Descriptor handle ranges all in one. This will consume copies for descriptor handle ranges,
    //! each range of which contain their own consecutive handles.
    //!
    //! \param PDevice
    //! \param NumSrcDescriptorStarts
    //! \param PSrcDescriptorHandleStarts
    //! \param PSrcDescriptorHandleSizes
    //! \param NumDstOffsetsInDescriptorCount
    //! \param NuDescriptorTableOuts
    //! \param PPDescriptorTables
    //! \return The resulting code.
    GResult CopyDescriptorRanges(ID3D12Device* PDevice,
                                 U32 NumSrcDescriptorStarts,
                                 D3D12_CPU_DESCRIPTOR_HANDLE* PSrcDescriptorHandleStarts,
                                 U32* PSrcDescriptorHandleSizes,
                                 U32* NumDstOffsetsInDescriptorCount,
                                 U32 NumDescriptorTablesOuts,
                                 DescriptorTable** PPDescriptorTables);

    //! Obtain the base cpu address from this descriptor pool.
    //!
    //! \return The base CPU address of the descriptor pool.
    D3D12_CPU_DESCRIPTOR_HANDLE GetBaseCPUAddress() const { return m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(); }
    
    //! Get the base gpu address from this descriptor pool.
    //! 
    //! \return The base GPU address of the descriptor pool.
    D3D12_GPU_DESCRIPTOR_HANDLE GetBaseGPUAddress() const { return m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(); }

    //! Reset the pool entirely. This sets our last available handles back to the beginning.
    void ResetPool() { m_LastCpuHandle = GetBaseCPUAddress(); m_LastGpuHandle = GetBaseGPUAddress(); }

    //! Get the GPU Handle address from the corresponding CPU handle address. Returns the base address
    //! if no possible to find the given Input CPU handle.
    //!
    //! \param Handle The CPU handle.
    //! \return The corresponding GPU handle. Returns the base address of the gpu descriptor if the given
    //!         inpute CPU handle is not within this descriptor table.
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUAddressFromCPUAddress(D3D12_CPU_DESCRIPTOR_HANDLE Handle);

private:
    //! Descriptor heap handle from native context.
    ID3D12DescriptorHeap* m_DescriptorHeap;

    //! The last cpu handle that is empty.
    D3D12_CPU_DESCRIPTOR_HANDLE m_LastCpuHandle;

    //! The corresponding last gpu handle in this descriptor pool.
    D3D12_GPU_DESCRIPTOR_HANDLE m_LastGpuHandle;

    //! Alingment size of the 
    UINT m_AlignmentSizeInBytes;
    
    //! Total size of the descriptor heap in bytes.
    UINT m_TotalDescriptorHeapSizeInBytes;
    
    //! Fast access to descriptor heap type.
    D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorHeapType;
};


class D3D12GPUMemoryManager {
public:
    static void InitializeSamplerPool(ID3D12Device* PDevice, U64 SizeInBytes);
    static void InitializeRtvDsvHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    static void InitializeCbvSrvUavHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    static void InitializeSceneMemoryHeap(ID3D12Device* PDevice, U64 SizeInBytes);
    static void InitializeScratchMemoryHeap(ID3D12Device* PDevice, U64 SizeInBytes);    

    static void InitializeUploadCbvSrvUavDescriptorHeaps(U32 NumHeaps, U32 NumTotalDescriptors);
    static void InitializeRtvDescriptorHeap(U32 NumTotalDescriptors);
    static void InitializeDsvDescriptorHeap(U32 NumTotalDescriptors);
    static void InitializeUploadDescriptorSamplerHeaps(U32 NumHeaps, U32 NumTotalDescriptors);

    //! Upload heaps that are used for rendering. 
    static void InitializeGPUCbvSrvUavDescriptorHeap(ID3D12Device* PDevice, U32 NumDescriptors);
    static void InitializeGPUSamplerDescriptorHeap(ID3D12Device* PDevice, U32 NumDescriptors);

    static MemoryPool& GetSamplerHeap();
    static MemoryPool& GetRtvDsvHeap();
    static MemoryPool& GetCbvSrvUavHeap();

    static MemoryPool& GetSceneMemoryHeap();
    static MemoryPool& GetScratchMemoryHeap();
    static MemoryPool& GetUploadMemoryHeap();
    static MemoryPool& GetReadbackMemoryHeap();


    static DescriptorPool& DescriptorPoolRtv();
    static DescriptorPool& DescriptorPoolDsv();
    static DescriptorPool& DescriptorPoolCbvSrvUav();
    static DescriptorPool& DescriptorPoolSampler();

    static DescriptorPool& UploadDescriptorPoolCbvSrvUav(U32 BufferIndex);
    static DescriptorPool& UploadDescriptorPoolSamplers(U32 BufferIndex);

};
} // Synthe 