// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Graphics/GraphicsStructs.hpp"
#include "Graphics/GraphicsCommandList.hpp"


namespace Synthe {


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


typedef enum MemoryType
{
    MemoryType_SCRATCH,
    MemoryType_SCENE,
    MemoryType_TEXTURE,
    MemoryType_RENDER_TARGETS_AND_DEPTH,
    MemoryType_BUFFER,
    MemoryType_UPLOAD,
    MemoryType_READBACK
} MemoryType;


//! Command list begin info, used to determine the behaviour, and the type of 
//! command list.
struct CommandListCreateInfo
{
    CommandListType Type;
    CommandListLevel Level;
};


class GraphicsDevice {
public:
    GraphicsDevice()
        : m_Features() { }

    virtual ~GraphicsDevice() { }
    virtual ResultCode Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                               const SwapchainConfig& SwapchainConfig) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CleanUp() { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode RecreateSwapchain() { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode Present() { return SResult_NOT_IMPLEMENTED; }

    virtual ResultCode CreateResource(GPUHandle* OutHandle, const ResourceCreateInfo* PCreateInfo) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateShaderResourceView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateConstantBufferView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateRenderTargetView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode CreateUnorderedAccessView(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    virtual ResultCode DestroyResource(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }
    virtual ResultCode DestroyShaderResourceView(GPUHandle Handle) { return SResult_NOT_IMPLEMENTED; }

    GraphicsDeviceFeatures GetDeviceFeatures() const { return m_Features; }

    virtual ResultCode CreateBuffer(GPUHandle* OutHandle) { return SResult_NOT_IMPLEMENTED; }

    //! Begin next buffer for rendering.
    virtual void Begin() { }
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