// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/Swapchain.hpp"
#include "Win32Common.hpp"

#include <vector>

namespace Synthe {


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


//! Frame Resources that involve the actual swapchain image, with its corresponding 
//! Render Target View handle.
struct FrameResource
{
    //! Queried swapchain Image.
    ID3D12Resource* PSwapchainImage;
    //! The Render Target Handle.
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle;
    
    //! The upload descriptor heaps to read from shadercode.
    
    //! Default constructor method for the Rtv Image.
    FrameResource(ID3D12Resource* PImage = nullptr, D3D12_CPU_DESCRIPTOR_HANDLE Rtv = BASE_CPU_DESCRIPTOR_ALLOC) 
        : RtvHandle(Rtv)
        , PSwapchainImage(PImage) { }
};


//! Swapchain Handler class. This is usually handled internally by the Graphics Device, but 
//! application may have control over it as well.
class D3D12Swapchain : public Swapchain {
public:

    //! Initialize the Swapchain object.
    void Initialize(const SwapchainConfig& Config, ID3D12Device* PDevice, IDXGIFactory2* PFactory);
    
    //! Resize the swapchain image count. This will not effect the buffering resources, unless the information is
    //! also different. In order to prevent recreation of buffer resources, use the 
    void Resize(const SwapchainConfig& Config) override;
    void CleanUp();

    const FrameResource& GetFrameResource(U32 FrameIndex) const { return m_FrameResources[FrameIndex]; }
    const BufferingResource& GetBufferingResource(U32 BufferIndex) const { return m_BufferingResources[BufferIndex]; }
    U32 GetNextFrameIndex();
    U32 GetBufferIndex() const { return BufferIndex; }

    ID3D12CommandQueue* GetBackBufferQueue() { return m_BackbufferQueue; }

    void SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex);

private:
    GResult CreateBackbufferQueue(ID3D12Device* PDevice);
    void CleanUpFrameResources();
    void CleanUpBufferingResources();
    void QueryFrames(ID3D12Device* PDevice, U32 ImageCount);
    void QueryBufferingResources(ID3D12Device* PDevice, U32 BufferingCount);
    
    IDXGISwapChain3* m_NativeHandle;
    std::vector<FrameResource> m_FrameResources;
    std::vector<BufferingResource> m_BufferingResources;
    ID3D12CommandQueue* m_BackbufferQueue;
    U32 BufferIndex;
};
} // Synthe 