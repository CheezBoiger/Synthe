// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Graphics/GraphicsStructs.hpp"
#include "Graphics/GraphicsCommandList.hpp"
#include "Graphics/PipelineState.hpp"


namespace Synthe {


class Swapchain;
class Fence;


//! Graphics device features, that are available from the device queried.
//! Typically GraphicsDevice object will have this object filled.
struct GraphicsDeviceFeatures
{
    GPUVendor Vendor;
    U64 DedicatedVideoMemoryInBytes;
    U64 DedicatedSystemMemoryInBytes;
    U64 SharedSystemMemory;
    B64 RayTracingCompatible            : 1,
        VariableRateShadingCompatible   : 1,
        ConservativeRasterCompatible    : 1,
        TiledResourcesCompatible        : 1,
        BarycentricsCompatible          : 1;
};


//! Difference memory types allocated by our graphics device.
//! Each of these types will have a default allocated size, but we may want to 
//! leave it defined by the programmer.
typedef enum MemoryType
{
    //! Scratch memory, mainly for additional buffer resource.
    MemoryType_SCRATCH,
    //! Scene data, should be quick.
    MemoryType_SCENE,
    //! Texture memory, for surface textures.
    MemoryType_TEXTURE,
    //! Internal memory dedicated to render targets and depth.
    MemoryType_RENDER_TARGETS_AND_DEPTH,
    //! Buffer data, mainly for vertex and index buffers.
    MemoryType_BUFFER,
    //! Memory intended for uploading, this memory is cleared every frame.
    MemoryType_UPLOAD,
    //! Memory intended for reading back from gpu. This memory is cleared every frame.
    MemoryType_READBACK
} MemoryType;


//! Command list begin info, used to determine the behaviour, and the type of 
//! command list.
struct CommandListCreateInfo
{
    //! We want to know the type of command list to create.
    CommandListType Type;
    //! Mainly whether this Commandlist is a primary, or bundle.
    CommandListLevel Level;
    //! Command List creation flags used to indicate what the behavior of the 
    //! command list will be.
    CommandListFlags Flags;
};


enum SubmitQueue
{
    //! Submit to Graphics queue.
    SubmitQueue_GRAPHICS,
    //! Submit to Asynchronous queue.
    SubmitQueue_ASYNC,
    //! Submit to copy queue.
    SubmitQueue_COPY
};


//! Command List Submit information, which is used for queue execution behavior.
//! Normally this info is a set of command lists that wait or signal given fences,
//! although no fences need be used.
struct CommandListSubmitInfo
{
    //! Array of command lists corresponding to NumCommandLists. 
    GraphicsCommandList** PCmdLists;
    //! The Queue type to submit our commandlists to.
    SubmitQueue QueueToSubmit;
    //! Number of fences to wait for.
    U32 NumWaitFences;
    //! Fences to wait on, if no fence defined, automatically 
    //! submitted. 
    Fence** WaitFences;
    //! Number of command lists to submit.
    U32 NumCommandLists;
    //! Number of Signal fences to signal.
    U32 NumSignalFences;
    //! Fences to signal.
    Fence** SignalFences; 
};


//! Graphics device is the interface for the Device we are using for rendering.
class GraphicsDevice {
public:
    GraphicsDevice()
        : m_Features() { }

    virtual ~GraphicsDevice() { }

    //! Initialize the Graphics Device. This means to initialize memory heaps, as well as banks and swapchain.
    //! 
    //! \param DeviceConfig Configurations for the Device to hint at.
    //! \param SwapchainConfig Configurations for the swapchain to hint at.
    //! \return SResult_OK if the call is successful. Any other code will be a failure.
    virtual ResultCode Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                               const SwapchainConfig& SwapchainConfig) { return SResult_NOT_IMPLEMENTED; }

    //! Clean up the Graphics Device resources, this includes all memory heaps and objects 
    //! handled by the device.
    //! 
    //! \return SResult_OK if the clean call was successfull.
    virtual ResultCode CleanUp() { return SResult_NOT_IMPLEMENTED; }

    //! Recreate the swapchain if needed. This is used for if the window is resizing, moving, minimized,
    //! or changing modes.
    //!
    //! \return SResult_OK if the swapchain was recreated successfully. Any other code will signify a 
    //!         failure.
    virtual ResultCode RecreateSwapchain() { return SResult_NOT_IMPLEMENTED; }

    //! Present the backbuffer to the screen. Typically this means to submit to the display engine
    //! to show our buffer (this doesn't mean it will display immediately, especially if multiple 
    //! buffers exist, this just means to queue when ready, so as to not block the main cpu thread.)
    //! 
    //! \return SResult_OK if the present call submit was successful. Any other code will be a failure.
    virtual ResultCode Present() { return SResult_NOT_IMPLEMENTED; }

    //! Create a raw resource object, which will return a GPU handle to the object.
    //! This object is used to represent the raw memory representation for the given dimensions.
    //! 
    //! \param OutHandle
    //! \param PCreateInfo
    //! \param PClearValue
    //! \return SResult_OK if the call successfully creates a resource object. On success, the OutHandle
    //!         will be initialized with the handle to the given resource that was created. On failure,
    //!         OutHandle will not be initialized, and the result code will return any other code for the 
    //!         given reason.
    virtual ResultCode CreateResource(GPUHandle* OutHandle, 
                                      const ResourceCreateInfo* PCreateInfo, 
                                      const ClearValue* PClearValue) { return SResult_NOT_IMPLEMENTED; }

    //! Create a shader resource view for a given resource.
    virtual ResultCode CreateShaderResourceView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Create a constant buffer view for a given resource.
    virtual ResultCode CreateConstantBufferView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Create a render target view handle for a given resource.
    //! 
    //! \param RTV
    //! \param OutHandle
    //! \return The result of the call. SResult_OK if the call was successfully executed, and OutHandle was 
    //!         generated. False otherwise, and OutHandle will not be supplied.
    virtual ResultCode CreateRenderTargetView(const RenderTargetViewCreateInfo& RTV, 
                                              GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Create a depth stencil view handle for a given resource.
    //!
    //! \param OutHandle
    //! \return The resulf of the call. SResult_OK if the call was successfully executed, and OutHandle was
    //!         generated. False otherwise, and OutHandle will not be supplied.
    virtual ResultCode CreateDepthStencilView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Create an unordered access view for a given resource.
    //!
    virtual ResultCode CreateUnorderedAccessView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Create a fence object.
    virtual ResultCode CreateFence(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! 
    virtual ResultCode DestroyResource(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }

    //!
    virtual ResultCode DestroyShaderResourceView(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }
    
    //!
    virtual ResultCode DestroyFence(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }

    //! Get this current device's features. GraphicsDevice must be initialized, before calling this
    //! getter.
    //!
    //! \return The graphics device features of this initialized device.
    GraphicsDeviceFeatures GetDeviceFeatures() const { return m_Features; }

    virtual ResultCode CreateBuffer(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Begin next buffer for rendering.
    //!
    virtual void Begin() { }

    //! End the frame buffer. This should be called after rendering and submitting.
    //!
    virtual void End() { }

    //! Get the overall dedicated heap size allocated for the device.
    //!
    //! \return The size of the dedicated heap in bytes.
    virtual U64 GetGPUHeapSizeInBytes() { return 0ULL; }

    //! Get the overall host heap size allocated for the device.
    //!
    //! \return The size of the host heap in bytes.
    virtual U64 GetCPUHeapSizeInBytes() { return 0ULL; }

    //! Get the amount of memory in bytes currently occupied by a memory pool.
    //!
    //! \param Key
    //! \return 
    virtual U64 GetCurrentUsedMemoryBytesInPool(U64 Key) { return 0ULL; }

    //! Get the total size of the memory pool in bytes.
    virtual U64 GetTotalSizeMemoryBytesForPool(U64 Key) { return 0ULL; }

    //! Create a command list for the application to use.
    //!
    //! \param Info
    //! \param PCmdList
    //! \return SResult_OK if the creation of the command list was a success.
    virtual ResultCode CreateCommandList(CommandListCreateInfo& Info, 
                                         GraphicsCommandList** PCmdList) { return SResult_NOT_IMPLEMENTED; }

    //! Submit CommandLists.
    //!
    //! \param NumSubmits
    //! \param PSubmitInfos
    //! \return 
    virtual ResultCode SubmitCommandLists(U32 NumSubmits, 
                                          const CommandListSubmitInfo* PSubmitInfos) { return SResult_NOT_IMPLEMENTED; }

    //! Destroy command lists.
    //!
    //! \param NumCommandLists
    //! \param CommandList
    //! \return
    virtual ResultCode DestroyCommandLists(U32 NumCommandLists, 
                                           GraphicsCommandList** CommandList) { return SResult_NOT_IMPLEMENTED; }

    //! Get the swapchain for this device.
    //!
    //! \return The swapchain object that corresponds to this device.
    virtual Swapchain* GetSwapchain() { return nullptr; }

    //! Create a graphics rasterization pipeline for use with rendering.
    //!
    //! \param OutHandle
    //! \param CreateInfo
    //! \return SResult_OK if the call succeeds.
    virtual ResultCode CreateGraphicsPipeline(GPUHandle* OutHandle, 
                                              const GraphicsPipelineStateCreateInfo& CreateInfo) { return SResult_NOT_IMPLEMENTED; }

    //! Create a compute pipeline for use with rendering and other tasks.
    //!
    //! \param OutHandle
    //! \param CreateInfo
    //! \return SResult if the call succeeds.
    virtual ResultCode CreateComputePipeline(GPUHandle* OutHandle,
                                            const ComputePipelineStateCreateInfo& CreateInfo) 
        { return SResult_NOT_IMPLEMENTED; }

    //! Create a hardware accelerated ray tracing pipeline for use with rendering.
    //! Note that this only works on hardware with supported ray tracing features.
    //!
    //! \param OutHandle
    //! \param CreateInfo
    //! \return 
    virtual ResultCode CreateRayTracingPipeline(GPUHandle* OutHandle,
                                                const RayTracingPipelineStateCreateInfo& CreateInfo) 
        { return SResult_NOT_IMPLEMENTED; }

    //! Create a gpu fence for use with syncronizing submits.
    //! 
    //! \param OutFence
    //! \return SResult_OK if the call succeeds, and the fence is created. Any other code
    //!         signals a failure, along with no value stored for fence.
    virtual ResultCode CreateFence(Fence** OutFence) { return SResult_NOT_IMPLEMENTED; }

    //! Create a Root signature that defines the layout of the descriptor 
    //! and resource layouts to be used for a given pipeline state.
    //!
    //! \param OutHandle
    //! \param CreateInfo
    //! \return SResult_OK if the function succeeds, and OutHandle is stored. Any other code
    //!         will output failure and no value is passed to OutHandle.
    virtual ResultCode CreateRootSignature(GPUHandle** OutHandle,
                                           const RootSignatureCreateInfo& CreateInfo) 
        { return SResult_NOT_IMPLEMENTED; }

    //! Create a Sampler for texture use.
    //! 
    //! \return SResult_ON if the function succeeds. Any other code will signify a failure.
    virtual ResultCode CreateSampler() { return SResult_NOT_IMPLEMENTED; }

protected:  
    GraphicsDeviceFeatures m_Features;
};


GraphicsDevice* CreateDeviceD3D12();
} // Synthe