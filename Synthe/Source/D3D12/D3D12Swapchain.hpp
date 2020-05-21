// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/Swapchain.hpp"
#include "Win32Common.hpp"

#include <vector>

namespace Synthe {


struct FrameResource
{
    ID3D12DescriptorHeap* PSrvCbvUavHeap;
    ID3D12DescriptorHeap* PRtvHeap;
    ID3D12DescriptorHeap* PDsvHeap;
    ID3D12CommandAllocator* PCommandAllocator;

    ID3D12Resource* PSwapchainImage;
    D3D12_CPU_DESCRIPTOR_HANDLE PRtvHandle;
};


class D3D12Swapchain : public Swapchain {
public:

    void Initialize(const SwapchainConfig& Config, ID3D12Device* PDevice, IDXGIFactory2* PFactory);
    void Resize(const SwapchainConfig& Config) override;
    void CleanUp();

    U32 GetNextFrameIndex();

    ID3D12CommandQueue* GetBackBufferQueue() { return m_BackbufferQueue; }

private:
    GResult CreateBackbufferQueue(ID3D12Device* PDevice);
    void QueryFrames();
    
    IDXGISwapChain3* m_NativeHandle;
    std::vector<FrameResource> m_FrameResources;
    ID3D12CommandQueue* m_BackbufferQueue;
};
} // Synthe 