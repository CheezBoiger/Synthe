// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/Swapchain.hpp"
#include "Win32Common.hpp"
#include "D3D12GPUManager.hpp"

#include <vector>

namespace Synthe {


struct BufferingResource
{
    ID3D12CommandAllocator* PCommandAllocator;
    ID3D12CommandList* DirectCommandList;
    DescriptorPool DescriptorPoolRtv;
    DescriptorPool DescriptorPoolDsv;
    DescriptorPool DescriptorPoolCbvSrvUav;
    DescriptorPool DescriptorPoolSampler;
    ID3D12Fence* PWaitFence;
    ID3D12Fence* PSignalFence;
    HANDLE FenceEventWait;
    HANDLE FenceEventSignal;
    U32 FenceSignalValue;
};


struct FrameResource
{
    ID3D12Resource* PSwapchainImage;
    D3D12_CPU_DESCRIPTOR_HANDLE PRtvHandle;
};


class D3D12Swapchain : public Swapchain {
public:

    void Initialize(const SwapchainConfig& Config, ID3D12Device* PDevice, IDXGIFactory2* PFactory);
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