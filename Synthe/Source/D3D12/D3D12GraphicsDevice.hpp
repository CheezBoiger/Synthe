// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once


#include "Win32Common.hpp"
#include "D3D12Swapchain.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Common/Memory/Allocator.hpp"


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
    ID3D12CommandList* DirectCommandList;
    ID3D12Fence* PWaitFence;
    ID3D12Fence* PSignalFence;
    HANDLE FenceEventWait;
    HANDLE FenceEventSignal;
    U32 FenceSignalValue;
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

    ResultCode Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                       const SwapchainConfig& SwaphchainConfig) override;
    ResultCode CleanUp() override;

    U32 GetBufferIndex() const { return m_BufferIndex; }

    ID3D12CommandQueue* GetBackBufferQueue() { return m_BackbufferQueue; }
    const BufferingResource& GetBufferingResource(U32 BufferIndex) const { return m_BufferingResources[BufferIndex]; }
    void SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex);

    U64 GetCurrentUsedMemoryBytesInPool(U64 Key) override;
    U64 GetTotalSizeMemoryBytesForPool(U64 Key) override;
private:
    
    ResultCode CreateBackbufferQueue();
    void CleanUpBufferingResources();
    void QueryBufferingResources(U32 BufferingCount);
    

    std::vector<BufferingResource> m_BufferingResources;
    ID3D12CommandQueue* m_BackbufferQueue;
    U32 m_BufferIndex;

    ID3D12Device*       m_Device;
    IDXGIFactory2*      m_PFactory;
    IDXGIAdapter1*      m_Adapter;
#if DIRECTML_COMPATIBLE
    IDMLDevice*         m_MLDevice;
#endif
    D3D12Swapchain      m_Swapchain;
};
} // Synthe