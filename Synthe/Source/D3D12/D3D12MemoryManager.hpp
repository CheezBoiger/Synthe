// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once

#include "Common/Memory/Allocator.hpp"

#include <unordered_map>

namespace Synthe {


class Allocator;


//! MemoryPool is the handler for internal API objects. This is in terms of the 
//! D3D12 Heap handler. Resources are created via this object, should they need
//! to be allocated within user managed pools.
class MemoryPool
{
public:
    MemoryPool()
        : m_Allocator(nullptr)
        , m_CurrentAllocatedBytes(0ULL)
        , m_Heap(nullptr)
        , m_TotalSizeInBytes(0ULL) { }

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
    ResultCode AllocateResource(ID3D12Device* PDevice,
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
    //! \return The resulting code OK, if the creation of the pool succeeds. Otherwise,
    //!         failure code will return.
    ResultCode Create(ID3D12Device* PDevice, 
                      Allocator* Allocator,
                      D3D12_HEAP_DESC& Desc);

    //! Free the resource that was allocated with this MemoryPool.
    //!
    //! \param PResource The Resource that will be freed.
    //! \return The resulting code. GResult_OK if the call succeeds, and the resource is 
    //!         invalid. Other code retured if failed.
    ResultCode FreeResource(ID3D12Resource* PResource);

    //! Release the MemoryPool's API handle. This is a clean up function.
    //! \return The resulting code. GResult_OK if the call successfully releases the internal
    //!         handle. Other code if the cleanup fails.
    ResultCode Release();

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


//! Resource State keeps track of the resource during recording and 
//! use.
struct ResourceState
{
    //! The native resource.
    ID3D12Resource* PResource;
    
    //! The current state of the resource.
    D3D12_RESOURCE_STATES State;
};

//! Memory manager handles all memory pool and allocator descriptions, that are 
//! used by the device, along with the application. The control here allows 
//! for full freedom of how we want to allocate our resources, which tend to be 
//! raw from the GPU standpoint, although some knowledge of a resource's description
//! can in fact help hardware vendors better optimize certain memory types.
class D3D12MemoryManager 
{
public:
    typedef U64 MemoryKeyID;

    enum AllocType 
    {
        AllocType_NEW,
        AllocType_LINEAR,
        AllocType_BUDDY,
        AllocType_FREELIST,
        AllocType_CUSTOM    //< Custom allocation type allows for user to specify their own inherited Allocator.
    };
    typedef U32 AllocT;

    //! Create an allocator concept for our memory pools.
    //!
    //! \param Key
    //! \param AllocatorType
    //! \param PAllocator Optional and only used if AllocType_CUSTOM is used.
    static void CreateAndRegisterAllocator(MemoryKeyID Key, 
                                           AllocT AllocatorType, 
                                           Allocator* PAllocator = nullptr);

    //! Get an Allocator at the Key location. Returns NULL if 
    //! no Allocator exists with the given key.
    static Allocator* GetAllocator(MemoryKeyID Key);

    //! Destroy memory pools allocated at Key.
    //! 
    //! \param Key
    //! \return SRESULT_OK if the function call is successful.
    static ResultCode DestroyMemoryPoolsAtKey(MemoryKeyID Key);

    //! Destroy an allocator at the Key location.
    static ResultCode DestroyAllocatorAtKey(MemoryKeyID Key);

    //! Get the desired Memory Pool with it's registered Key.
    static MemoryPool* GetMemoryPool(MemoryKeyID Key);

    //! Create and register memory pools for the given key. This will
    //! also allow for buffered memory pools as well.
    static ResultCode CreateAndRegisterMemoryPool(MemoryKeyID Key);

    //! Get the total GPU memory reserved by this memory manager. This is the total
    //! memory pool from all pools of this type.
    static U64 GetTotalGPUReservedInBytes() { return k_TotalGPUMemoryBytes; }
    
    //! Get the total CPU memory reserved by this memory manager. This is the total
    //! memory pool from all pools of this type.
    static U64 GetTotalCPUReservedInBytes() { return k_TotalCPUMemoryBytes; }

    //! Cache the native gpu resource, once it has been allocated.
    static ResultCode CacheNativeResource(GPUHandle Key, ID3D12Resource* PResource, D3D12_RESOURCE_STATES InitialState);

    //! Get the cached native resource, if one exists. Otherwise, an error should result.
    //!
    static ResultCode GetNativeResource(GPUHandle Key, ResourceState* POutResource);
    
    //! Updates the state of the resource.
    //!
    //! \param Key
    //! \param State
    //! \return 
    static ResultCode UpdateResourceState(GPUHandle Key, D3D12_RESOURCE_STATES State);
    
protected:
    //! Our friends!
    friend class MemoryPool;

    //! Get the cached resource size. Caches an allocation block if not doesn't already exist.
    //! This function should prevent querying from the driver if need be.
    //!
    //! \param PDevice
    //! \param Desc
    //! \param Out
    //! \return SResult_OK if the search is successful.
    static ResultCode GetCachedResourceSize(ID3D12Device* PDevice, 
                                            D3D12_RESOURCE_DESC& Desc, 
                                            D3D12_RESOURCE_ALLOCATION_INFO* Out);

    //! Total memory reserved by GPU context.
    static U64 k_TotalGPUMemoryBytes;

    //! Total memory reserved by CPU context.
    static U64 k_TotalCPUMemoryBytes;
};
} // Synthe