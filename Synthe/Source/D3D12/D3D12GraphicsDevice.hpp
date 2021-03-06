// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once

#include "Graphics/GraphicsDevice.hpp"
#include "Common/Memory/Allocator.hpp"

#include "Win32Common.hpp"
#include "D3D12Swapchain.hpp"
#include "D3D12CommandList.hpp"
#include "D3D12Resource.hpp"

#include <list>
#include <unordered_map>
#include <map>


namespace Synthe {


class D3D12Fence;


enum DescriptorHeapType
{
    // GPU based cbv srv uav heap.
    DescriptorHeapType_CBV_SRV_UAV,
    // CPU controlled cbv srv uav heap.
    DescriptorHeapType_CBV_SRV_UAV_UPLOAD,
    // CPU controlled sampler heap.
    DescriptorHeapType_SAMPLER_UPLOAD,
    // GPU controlled sampler heap.
    DescriptorHeapType_SAMPLER,
    // CPU controlled rtv heap.
    DescriptorHeapType_RTV,
    // CPU controlled dsv heap.
    DescriptorHeapType_DSV
};


//! The Buffering resources that are used as part of the number of allowed frames in flight.
struct BufferingResource
{
    ID3D12CommandAllocator* PCommandAllocator;
    ID3D12Fence* PWaitFence;
    HANDLE FenceEventWait;
    U64 FenceWaitValue;
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
        , m_GraphicsQueue(nullptr)
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

    //! Get the graphics queue.
    ID3D12CommandQueue* GetGraphicsQueue() { return m_GraphicsQueue; }

    //! Submit call to present the final back buffer to window.
    ResultCode Present() override;

    //! Create our resource.
    //!
    //! \param Out
    //! \param PCreateInfo
    //! \param PCreateInfo
    //! \param PClearValue
    //! \return 
    ResultCode CreateResource(GPUHandle* Out, 
                              const ResourceCreateInfo* PCreateInfo, 
                              const ClearValue* PClearValue) override;

    //! Get the buffering resource corresponding to the buffer index.
    const BufferingResource& GetBufferingResource(U32 BufferIndex) const { return m_BufferingResources[BufferIndex]; }
    void SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex);

    U64 GetCurrentUsedMemoryBytesInPool(U64 Key) override;
    U64 GetTotalSizeMemoryBytesForPool(U64 Key) override;

    //! Submit command lists.
    ResultCode SubmitCommandLists(U32 NumSubmits, 
                                  const CommandListSubmitInfo* PSubmitInfos) override;

    //! Create a D3D12 native command list.
    ResultCode CreateCommandList(CommandListCreateInfo& Info, 
                                 GraphicsCommandList** PList) override;

    //! Create a D3D12 render target view. This will be used for all render targets.
    ResultCode CreateRenderTargetView(const RenderTargetViewCreateInfo& RTV, 
                                      GPUHandle* OutHandle) override;

    //! Create a D3D12 shader resource view. This will be used for all shader resource view targets.
    ResultCode CreateShaderResourceView(const ShaderResourceViewCreateInfo& SRV, 
                                        GPUHandle* OutHandle) override;

    //! Create a D3D12 depth stencil view. This will be used for all depth stencil targets.
    ResultCode CreateDepthStencilView(const DepthStencilViewCreateInfo& DSV,
                                      GPUHandle* OutHandle) override;

    //!
    ResultCode CreateRootSignature(RootSignature** PRootSignature, const RootSignatureLayoutInfo& CreateInfo) override;

    //! 
    ResultCode AllocateDescriptorSets(U32 NumDescriptorSets, 
                                      DescriptorSet** PDescriptorSets,
                                      DescriptorSetLayoutInfo* PLayouts) override;

    //! Create graphics pipeline.
    ResultCode CreateGraphicsPipeline(PipelineState** OutPipelineState, 
                                      const GraphicsPipelineStateCreateInfo& CreateInfo) override;

    //! Create compute pipeline.
    ResultCode CreateComputePipeline(PipelineState** OutPipelineState,
                                     const ComputePipelineStateCreateInfo& CreateInfo) override;


    //! Begin the frame. This will prepare resources, along with prepare command lists and 
    //! other buffering resources.
    void Begin() override;

    //! End the current frame. This will submit a signal to the graphics queue, as well as 
    //! move to the next frame, so be sure to call Present() before calling End().
    void End() override;

    //! Wait for the graphics accelerator.
    void WaitOnGPU();

    //! Get the DXGI swapchain.
    //!
    //! \return The swapchain object that is mapped to this device object.
    Swapchain* GetSwapchain() override { return &m_Swapchain; }

    ResultCode DestroyCommandLists(U32 NumCommandLists, GraphicsCommandList** CommandLists) override;

    //! Create a D3D12 native GPU fence.
    //!
    //! \param OutFence
    //! \return 
    ResultCode CreateFence(Fence** OutFence) override;

    //! Get the native advanced device.
    //!
    ID3D12Device5* GetNativeAdv() { return m_AdvDevice; }
    
    //! Get the native device.
    //!
    ID3D12Device* GetNative() { return m_Device; }

private:
    //! Creates the back buffer queue.
    ResultCode CreateGraphicsQueue();

    //! Creates the asynchronous queue.
    ResultCode CreateAsyncQueue();

    //! Creates the copy queue.
    ResultCode CreateCopyQueue();

    void CleanUpFences();

    //! Cleans up buffering resources.
    void CleanUpBufferingResources();

    //! Queries for frame in flight buffers.
    void QueryBufferingResources(U32 BufferingCount);
    
    //! Release the asynchronous queue.
    void ReleaseAsyncQueue();
    
    //! Release the copy queue.
    void ReleaseCopyQueue();

    //! Our buffering resources.
    std::vector<BufferingResource>              m_BufferingResources;

    //! 
    ID3D12CommandQueue*                         m_GraphicsQueue;
    
    //! Asynchronous queue should any be available.
    ID3D12CommandQueue*                         m_AsyncQueue;

    //! Copy queue should any be available.
    ID3D12CommandQueue*                         m_CopyQueue;

    U32                                         m_BufferIndex;

    std::map<GPUHandle, D3D12DescriptorSet*>    m_DescriptorSets;
    std::list<D3D12GraphicsCommandList*>        m_PerFrameCommandLists;
    std::unordered_map<GPUHandle, D3D12Fence*>  m_Fences;
    D3D12GraphicsCommandList                    m_BackbufferCommandList;

    ID3D12Device*                               m_Device;
    ID3D12Device5*                              m_AdvDevice;
    IDXGIFactory2*                              m_PFactory;
    IDXGIAdapter1*                              m_Adapter;
#if DIRECTML_COMPATIBLE
    IDMLDevice*                                 m_MLDevice;
#endif
    D3D12Swapchain                              m_Swapchain;
    D3D12_RESOURCE_HEAP_TIER                    m_ResourceHeapTier;
};
} // Synthe