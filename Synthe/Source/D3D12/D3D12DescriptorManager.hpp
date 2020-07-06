// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once 

#include "Win32Common.hpp"
#include "Common/Memory/Allocator.hpp"

#include <unordered_map>

namespace Synthe {


typedef U64 DescriptorKeyID;


//! Descriptor table is an object that holds the handle to a base address of a descriptor heap,
//! along with the size of the table. These are used to represent arrays of descriptors to be 
//! bound on render command recording.
struct DescriptorTable
{
    DescriptorTable(D3D12_CPU_DESCRIPTOR_HANDLE StartAddress = BASE_CPU_DESCRIPTOR_ALLOC,
                    U64 SizeInBytes = 0ULL)
        : StartingAddress(StartAddress)
        , TableSizeInBytes(SizeInBytes) { }

    //! The starting CPU address of the descriptor table.
    D3D12_CPU_DESCRIPTOR_HANDLE StartingAddress;
    
    //! The end CPU address of the descriptor table.
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
    ResultCode Create(ID3D12Device* PDevice, 
                      D3D12_DESCRIPTOR_HEAP_TYPE Type, 
                      D3D12_DESCRIPTOR_HEAP_FLAGS Flags, 
                      U32 NumDescriptors);

    //! Releases the descriptor pool handle of the D3D12 Descriptor heap.
    //!
    //! \return The resulting code. GResult_OK if descriptor pool was successfully cleaned up.
    //!         Any other code otherwise should signal a failure.
    ResultCode Release();
    
    //! Allocate and reserve space for a descriptor table.
    //!
    //! \param OutTable
    //! \param SizeInBytes
    //! \return SResult_OK if the call succeeds.
    ResultCode AllocateDescriptorTable(DescriptorTable* OutTable, U64 SizeInBytes);

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
    ResultCode CopyDescriptorsRange(ID3D12Device* PDevice,
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
    ResultCode CopyDescriptorsRangeConsecutive(ID3D12Device* PDevice,
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
    ResultCode CopyDescriptorRanges(ID3D12Device* PDevice,
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

    //! Get the CPU address via gpu address. Returns the base CPU address if not possible to find the given
    //! input gpu handle.
    //! 
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAddressFromGPUAddress(D3D12_GPU_VIRTUAL_ADDRESS Address);

    //! Get the CPU address with descriptor count offset.
    //!
    //! \param DescriptorCountOffset
    //! \return The CPU Address with the descriptor offset.
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAddressWithDescriptorCount(UINT DescriptorCountOffset);
    
    //! Get the CPU descriptor offset with actual bytes. This must be aligned with the actual incremental
    //! size of the descriptor heap. This can be found with GetDescriptorHeapType() call.
    //!
    //! \param OffsetInBytes The byte offset from the base of the descriptor heap.
    //! \return The CPU Address handle with the given offset.
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAddressWithByteOffset(UINT64 OffsetInBytes);

    //! Get the current number of descriptor heaps in this pool.
    //!
    //! \return The current number of descriptors registered.
    UINT GetCurrentNumberOfDescriptors() const { return m_CurrentNumberOfDescriptors; }

    //! Get the alignment size of the descriptor heap in bytes.
    //!
    //! \return The alignment size in bytes.
    U64 GetAlignmentSizeInBytes() const { return static_cast<U64>(m_AlignmentSizeInBytes); }

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

    //! Current number of descriptors registered in this descriptor heap.
    UINT m_CurrentNumberOfDescriptors;
};


class D3D12DescriptorManager 
{
public:

    //! Create and Register descriptor pools for the given key. This will
    //! also allow for buffered memory pools as well.
    //!
    //! \param Key
    //! \param NumPools
    //! \return Returns GResult_OK if the creation and registration completes. Any other result code
    //!         will be returned if the function call fails.
    static ResultCode CreateAndRegisterDescriptorPools(DescriptorKeyID Key, U32 NumPools);
    
    //! Get the desired Descriptor Pool with it's registered Key.
    //! 
    //! \param Key
    //! \param Index
    //! \return The DescriptorPool object pointer. Returns null if not found.
    static DescriptorPool* GetDescriptorPool(DescriptorKeyID Key, U32 Index = 0);
    
    //! Destroy descriptor pools allocated at Key.
    //! 
    //! \param Key
    //! \return GResult_OK if the descriptor pools at location Key were successfully destroyed.
    static ResultCode DestroyDescriptorPoolsAtKey(DescriptorKeyID Key);

    //! 
    static ResultCode CacheDescriptorToResource(GPUHandle Descriptor, GPUHandle Resource);

    //!
    static ResultCode GetCachedResourceWithDescriptor(GPUHandle Descriptor, GPUHandle* Resource);

    //!
    static ResultCode RemoveCachedDescriptorToResource(GPUHandle Descriptor);

    //!
    static ResultCode CacheDescriptorTable(DescriptorKeyID Key, DescriptorTable& Table);
    
    //!
    static ResultCode GetDescriptorTable(DescriptorKeyID Key, DescriptorTable* Table);
    
    //!
    static ResultCode RemoveDescriptorTable(DescriptorKeyID Key);
};
} // Synthe 