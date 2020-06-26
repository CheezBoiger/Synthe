// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once

#include "Graphics/GraphicsDevice.hpp"
#include "Common/Memory/Allocator.hpp"

#include "Win32Common.hpp"
#include "D3D12Swapchain.hpp"
#include "D3D12CommandList.hpp"


namespace Synthe {


enum DescriptorType
{
    // GPU based cbv srv uav heap.
    DescriptorType_CBV_SRV_UAV,
    // CPU controlled cbv srv uav heap.
    DescriptorType_CBV_SRV_UAV_UPLOAD,
    // CPU controlled sampler heap.
    DescriptorType_SAMPLER_UPLOAD,
    // GPU controlled sampler heap.
    DescriptorType_SAMPLER,
    // CPU controlled rtv heap.
    DescriptorType_RTV,
    // CPU controlled dsv heap.
    DescriptorType_DSV
};


//! The Buffering resources that are used as part of the number of allowed frames in flight.
struct BufferingResource
{
    ID3D12CommandAllocator* PCommandAllocator;
    ID3D12Fence* PWaitFence;
    ID3D12Fence* PSignalFence;
    HANDLE FenceEventWait;
    HANDLE FenceEventSignal;
    U32 FenceSignalValue;
    U32 FenceWaitValue;
};


//! D3D12 Graphics Device. This handles the API layer of the graphics accelerator.
//!
class D3D12GraphicsDevice : public GraphicsDevice {
public:
    D3D12GraphicsDevice()
        : m_PFactory(nullptr)
        , m_Device(nullptr)
        , m_Adapter(nullptr)
        , m_BufferIndex(0ULL)
        , m_BackbufferQueue(nullptr)
#if DIRECTML_COMPATIBLE
        , m_MLDevice(nullptr)
#endif 
        { }

    //! Initialize the D3D12 Context for this graphics device.
    ResultCode Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                       const SwapchainConfig& SwaphchainConfig) override;

    //! Clean up the Graphics Device context.
    ResultCode CleanUp() override;

    //! The current buffering index. This specifies the current state of resources
    //! to use when rendering.
    //!
    //! \return The current buffer index that corresponds to in flight frames.
    U32 GetCurrentBufferIndex() const { return m_BufferIndex; }

    //! The back buffer queue that corresponds to the back buffers.
    ID3D12CommandQueue* GetBackBufferQueue() { return m_BackbufferQueue; }

    //! Submit call to present the final back buffer to window.
    ResultCode Present() override;

    //! Create our resource.
    ResultCode CreateResource(GPUHandle* Out, const ResourceCreateInfo* PCreateInfo, const ClearValue* PClearValue) override;

    //! Get the buffering resource corresponding to the buffer index.
    const BufferingResource& GetBufferingResource(U32 BufferIndex) const { return m_BufferingResources[BufferIndex]; }
    void SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex);

    U64 GetCurrentUsedMemoryBytesInPool(U64 Key) override;
    U64 GetTotalSizeMemoryBytesForPool(U64 Key) override;

    void Begin() override;
    void End() override;
private:
    //! Creates the back buffer queue.
    ResultCode CreateBackbufferQueue();

    //! Cleans up buffering resources.
    void CleanUpBufferingResources();

    //! Queries for frame in flight buffers.
    void QueryBufferingResources(U32 BufferingCount);
    
    //! Our buffering resources.
    std::vector<BufferingResource>      m_BufferingResources;

    //! 
    ID3D12CommandQueue*                 m_BackbufferQueue;
    D3D12GraphicsCommandList            m_BackBufferCommandList;

    U32                                 m_BufferIndex;

    ID3D12Device*                       m_Device;
    IDXGIFactory2*                      m_PFactory;
    IDXGIAdapter1*                      m_Adapter;
#if DIRECTML_COMPATIBLE
    IDMLDevice*                         m_MLDevice;
#endif
    D3D12Swapchain                      m_Swapchain;
    D3D12_RESOURCE_HEAP_TIER            m_ResourceHeapTier;
};
} // Synthe