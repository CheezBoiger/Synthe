// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Graphics/GraphicsStructs.hpp"
#include "Graphics/GraphicsCommandList.hpp"


namespace Synthe {


//! Graphics device features, that are available from the device queried.
//! Typically GraphicsDevice object will have this object filled.
struct GraphicsDeviceFeatures
{
    GPUVendor Vendor;
    U64 DedicatedVideoMemoryInBytes;
    U64 DedicatedSystemMemoryInBytes;
    U64 SharedSystemMemory;
    B64 RayTracingCompatible : 1,
        VariableRateShadingCompatible : 1,
        ConservativeRasterCompatible : 1,
        TiledResourcesCompatible : 1,
        BarycentricsCompatible : 1;
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

    virtual ResultCode CreateShaderResourceView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateConstantBufferView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateRenderTargetView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateUnorderedAccessView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    virtual ResultCode DestroyResource(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode DestroyShaderResourceView(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }

    //! Get this current device's features. GraphicsDevice must be initialized, before calling this
    //! getter.
    //!
    //! \return The graphics device features of this initialized device.
    GraphicsDeviceFeatures GetDeviceFeatures() const { return m_Features; }

    virtual ResultCode CreateBuffer(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Begin next buffer for rendering.
    virtual void Begin() { }

    //! End the frame buffer. This should be called after rendering and submitting.
    virtual void End() { }

    virtual U64 GetGPUHeapSizeInBytes() { return 0ULL; }
    virtual U64 GetCPUHeapSizeInBytes() { return 0ULL; }

    virtual U64 GetCurrentUsedMemoryBytesInPool(U64 Key) { return 0ULL; }
    virtual U64 GetTotalSizeMemoryBytesForPool(U64 Key) { return 0ULL; }

    virtual ResultCode CreateCommandList(CommandListCreateInfo& Info, GraphicsCommandList* PCmdList) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode SubmitCommandLists() { return SResult_NOT_IMPLEMENTED; }

protected:  
    GraphicsDeviceFeatures m_Features;
};


GraphicsDevice* CreateDeviceD3D12();
} // Synthe