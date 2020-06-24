// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/Swapchain.hpp"
#include "Win32Common.hpp"

#include <vector>

namespace Synthe {

class DescriptorPool;

//! Frame Resources that involve the actual swapchain image, with its corresponding 
//! Render Target View handle.
struct FrameResource
{
    //! Queried swapchain Image.
    ID3D12Resource* PSwapchainImage;
    
    //! The upload descriptor heaps to read from shadercode.
    D3D12_CPU_DESCRIPTOR_HANDLE ImageRTV;
    
    //! Default constructor method for the Rtv Image.
    FrameResource(ID3D12Resource* PImage = nullptr) 
        : PSwapchainImage(PImage)
        , ImageRTV(BASE_CPU_DESCRIPTOR_ALLOC) { }
};


//! Swapchain Handler class. This is usually handled internally by the Graphics Device, but 
//! application may have control over it as well.
class D3D12Swapchain : public Swapchain {
public:
    D3D12Swapchain() 
        : m_NativeHandle(nullptr) 
        , m_PresentFlags(0)
    {
    }

    //! Initialize the Swapchain object.
    void Initialize(const SwapchainConfig& Config, ID3D12CommandQueue* PBackBufferQueue, IDXGIFactory2* PFactory);

    //! Build RTVs.
    void BuildRTVs(ID3D12Device* PDevice, DescriptorPool* PPool);

    //! Resize the swapchain image count. This will not effect the buffering resources, unless the information is
    //! also different. In order to prevent recreation of buffer resources. 
    //!
    void Resize(const SwapchainConfig& Config) override;

    //! Clean up.
    //!
    void CleanUp();

    const FrameResource& GetFrameResource(U32 FrameIndex) const { return m_FrameResources[FrameIndex]; }

    //! Get the next frame index, this should be called after presentation.
    //!
    //! \return The next frame index.
    U32 GetNextFrameIndex();

    //! Get the total number of allocated frames in this swapchain.
    //!
    //! \return The total number of frames.
    U32 GetTotalFrames() const { return static_cast<U32>(m_FrameResources.size()); }

    //! Present the current back buffer to the user, depending on the flag set when initialized,
    //! will determine if this call will either wait for submittal, or not.
    //! 
    //! \return SResult_OK if the present submit is sent, otherwise GResult_FAILED.
    ResultCode Present() override;

private:
    void CleanUpFrameResources();
    void QueryFrames(U32 ImageCount);

    
    IDXGISwapChain3* m_NativeHandle;
    std::vector<FrameResource> m_FrameResources;
    UINT m_PresentFlags;
    
};
} // Synthe 