// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

namespace Synthe {


typedef U64 GPUHandle;
#define SYNTHE_GPU_NO_HANDLE 0ULL


enum GraphicsAPI
{
    GRAPHICS_API_DIRECT3D12,
    GRAPHICS_API_DIRECT3D11
};


//! GPU vendor identification.
enum GPUVendor
{
    //! Unknown vendor.
    GPUVendor_UNKNOWN,
    //! Vendor NVidia.
    GPUVendor_NVIDIA =      ( 1 << 0 ),
    //! Vendor Intel.
    GPUVendor_INTEL =       ( 1 << 1 ),
    //! Vendor Advanced Micro Devices.
    GPUVendor_AMD =         ( 1 << 2 ),
    //! Vendor Qualcomm.
    GPUVendor_QUALCOMM =    ( 1 << 3 ),
    //! Vendor Microsoft.
    GPUVendor_MICROSOFT =   ( 1 << 4 ),
    //! Any vendor.
    GPUVendor_ANY =         ( 0xFFFFFFFF )
};


enum GResult 
{
    GResult_FAILED = -89999,
    GResult_INITIALIZATION_FAILURE,
    GResult_LOST_DEVICE,
    GResult_GPU_MEMORY_CORRUPTION,
    GResult_DEVICE_CREATION_FAILURE,
    GResult_ROOT_SIGNATURE_CREATION_ERROR,
    GResult_ROOT_SIGNATURE_SERIALIZATION_ERROR,
    GResult_UNKNOWN
};



enum SwapchainFlags
{
    SwapchainFlags_NONE,
    SwapchainFlags_ALLOW_TEARING
};


//! Swapchain configuration to allow for setting up and initializing the swapchain.
//! This includes the render resolution, number of frames available, and number of buffers
//! to be used in-flight, as well as vsync and other configurations.
struct SwapchainConfig
{
    //! Width of the swapchain images.
    U32 Width;
    //! Height of the swapchain images.
    U32 Height;
    //! Number of swapchain frames to create. This allows more frames to be 
    //! available in the swapchain resevoir without stalling the swapchain display engine.
    //! So while buffering syncronizes with resources in the GPU, the frame count syncronizes with
    //! the display engine of your local machine.
    U32 NumFrames;
    //! Format of the Swapchain images.
    U32 Format;
    //! Native handle to a Windows application window.
    U64 NativeWinHandle;
    //! Bool value if the WinHandle is in windowed mode.
    U32 Windowed;
    //! Swapchain flags to be used for the application.
    SwapchainFlags Flags;
    //! Buffering is independent of Swapchain Frame count. This means that 
    //! Resources are syncronized based on this value, not the frame count granted
    //! to the application. In other words, this value determines how many frames 
    //! can be in-flight, before synronization is required. 
    //! Note: The larger the buffering value, the more memory required for each buffering
    //!       resource.
    U32 Buffering;
    //! Enable Vertical Syncronization.
    B32 VSync;
};


enum GFormat
{
    GFormat_UNKNOWN,
    GFormat_R8G8B8A8_UNORM,
    GFormat_R16G16B16A16_FLOAT,
    GFormat_R16G16_FLOAT,
    GFormat_R32_FLOAT,
    GFormat_D32_FLOAT,
    GFormat_D24_UNORM_S8_UINT,
    GFormat_R10G10B10A2_UNORM,
    GFormat_R11G11B10_FLOAT
};

typedef U32 PixelFormat;

enum GraphicsDeviceFlags
{
    GraphcisDeviceFlags_NONE =                          ( 0 ),
    GraphicsDeviceFlags_SOFTWARE_RASTERIZER_BIT =       ( 1 << 0 ),
    GraphicsDeviceFlags_RAYTRACING_HARDWARE_BIT =       ( 1 << 1 ),
    GraphicsDeviceFlags_MACHINE_LEARNING_HARWARE_BIT =  ( 1 << 2 ),
    GraphicsDeviceFlags_VARIABLE_RATE_SHADING_BIT =     ( 1 << 3 ),
    GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT =       ( 1 << 4 ),
    GraphicsDeviceFlags_TILED_RESOURCES_BIT =           ( 1 << 5 ),
    GraphicsDeviceFlags_BARYCENTRIC_COORDINATES_BIT =   ( 1 << 6 )
};


enum PipelineStateType
{
    PipelineStateType_GRAPHICS,
    PipelineStateType_COMPUTE,
    PipelineStateType_RAYTRACING
};


//! Graphics device configuration info. This is to be used for initializing the
//! device before use, to allow desired hardware or features.
struct GraphicsDeviceConfig
{
    //! Desired GPU Vendor.
    GPUVendor DesiredVendor;
    //! Desired Flags.
    GraphicsDeviceFlags DesiredFlags;
    //! True if the specified desires are required.
    B64 DesiresRequired : 1;
    //! Enable GPU validation for debugging.
    B64 EnableDeviceDebugLayer : 1;
    //! Maximum device memory in bytes, for texture Pool.
    U64 TexturePoolMemoryInBytes;
    //! Maximum device memory in bytes, for buffer pool.
    U64 BufferPoolMemoryInBytes;
    //! Maximum memory in bytes, for upload pool. This is memory pool used for 
    //! uploading to device local memory.
    U64 UploadPoolMemoryInBytes;
    //! Maximum memory in bytes, for readback. This should be small/none if gpu read back is not necessary.
    U64 ReadBackPoolMemoryInBytes;
    //! Maximum memory in bytes, for device render surfaces. This includes render targets,
    //! and depth stecil targets.
    U64 RenderTargetPoolMemoryInBytes;
    //! Maximum memory in bytes, for scratch pool.
    U64 ScratchPoolMemoryInBytes;
    //! Maximum memory in bytes, for shader resource surfaces.
    U64 ShaderResourceMemoryInBytes;
    
};


struct ClearColorValue
{
    float R, G, B, A;
};


struct TargetBounds
{
    U32 Left;
    U32 Top;
    U32 Right;
    U32 Bottom;
};


struct InstancedKey
{
    
};


typedef enum ResourceUsage
{
    ResourceUsage_VERTEX_BUFFER         = ( 1 << 0 ),
    ResourceUsage_INDEX_BUFFER          = ( 1 << 1 ),
    ResourceUsage_UNORDERED_ACCESS      = ( 1 << 2 ),
    ResourceUsage_SHADER_RESOURCE       = ( 1 << 3 ),
    ResourceUsage_DEPTH_STENCIL         = ( 1 << 4 ),
    ResourceUsage_RENDER_TARGET         = ( 1 << 5 )
} ResourceUsage;

typedef U32 ResourceUsageFlags;

typedef enum ResourceDimension
{
    ResourceDimension_BUFFER,
    ResourceDimension_TEXTURE1D,
    ResourceDimension_TEXTURE2D,
    ResourceDimension_TEXTURE3D,
} ResourceDimension;


typedef struct ClearValue {
    R32         Color[4];
    R32         Depth;
    U8          Stencil;
} ClearValue;


struct ResourceCreateInfo
{
    ResourceDimension       Dimension;
    PixelFormat                  ResourceFormat;
    U64                     Width;
    U32                     Height;
    U16                     DepthOrArraySize;
    U16                     Mips;
    ResourceUsageFlags      Usage;
    U16                     SampleCount;
    U16                     SampleQuality;
};


enum RTVViewDimension
{
    RTVViewDimension_BUFFER,
    RTVViewDimension_TEXTURE_1D,
    RTVViewDimension_TEXTURE_1D_ARRAY,
    RTVViewDimension_TEXTURE_2D,
    RTVViewDimension_TEXTURE_2DMS,
    RTVViewDimension_TEXTURE_2D_ARRAY,
    RTVViewDimension_TEXTURE_2DMS_ARRAY,
    RTVViewDimension_TEXTURE_3D
};


struct RTVBufferInfo
{
    U64 FirstElement;
    U32 NumElements;
};


struct RTVTexture1DInfo
{
    U32 MipSlice;
};


struct RTVTexture1DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipSlice;
};


struct RTVTexture2DInfo
{
    U32 MipSlice;
    U32 PlaneSlice;
};


struct RTVTexture2DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipSlice;
    U32 PlaneSlice;
};


struct RTVTexture3DInfo
{
    U32 FirstWSlice;
    U32 MipSlice;
    U32 WSize;
};


struct RTVTexture2DMSArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
};


//! Render Target View information.
struct RenderTargetViewCreateInfo
{
    GPUHandle ResourceHandle;
    RTVViewDimension Dimension;
    PixelFormat Format;
    union 
    {
        RTVBufferInfo Buffer;
        RTVTexture1DInfo Texture1D;
        RTVTexture1DArrayInfo Texture1DArray;
        RTVTexture2DInfo Texture2D;
        RTVTexture2DArrayInfo Texture2DArray;
        RTVTexture2DMSArrayInfo Texture2DMSArray;
        RTVTexture3DInfo Texture3D;
    };
};


enum DSVViewDimension
{
    DSVViewDimension_TEXTURE2D,
    DSVViewDimension_TEXTURE2D_ARRAY,
    DSVViewDimension_TEXTURE1D,
    DSVViewDimension_TEXTURE1D_ARRAY,
    DSVViewDimension_TEXTURE2DMS,
    DSVViewDimension_TEXTURE2DMS_ARRAY
};


struct DSVTexture1DInfo
{
    U32 MipSlice;
};


struct DSVTexture1DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipSlice;
};


struct DSVTexture2DInfo
{
    U32 MipSlice;
};


struct DSVTexture2DMSArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
};


struct DSVTexture2DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipSlice;
};


//! Depth Stencil View information.
struct DepthStencilViewCreateInfo
{
    GPUHandle ResourceHandle;
    PixelFormat Format;
    DSVViewDimension Dimension;
    union 
    {
        DSVTexture1DInfo Texture1D;
        DSVTexture1DArrayInfo Texture1DArray;
        DSVTexture2DArrayInfo Texture2DArray;
        DSVTexture2DInfo Texture2D;
        DSVTexture2DMSArrayInfo Texture2DMSArray;
    };
};


enum SRVViewDimension
{
    SrvViewDimension_BUFFER,
    SrvViewDimension_TEXTURE1D,
    SrvViewDimension_TEXTURE2D,
    SrvViewDimension_TEXTURE2DMS,
    SrvViewDimension_TEXTURE3D,
    SrvViewDimension_TEXTURE2D_ARRAY,
    SrvViewDimension_TEXTURE2DMS_ARRAY,
    SrvViewDimension_TEXTURE1D_ARRAY,
    SrvViewDimension_TEXTURE_CUBE,
    SrvViewDimension_TEXTURE_CUBE_ARRAY,
    SrvViewDimension_RAYTRACING_ACCELERATION_STRUCTURE
};


struct SrvTexture2DInfo
{
    U32 MipLevels;
    U32 MostDetailedMip;
    U32 PlaneSlice;
    R32 ResourceMinLODClamp;
};


struct SrvBufferInfo
{
    U64 FirstElement;
    U32 Flags;
    U32 NumElements;
    U32 StructureByteStride;
};


struct SrvRayTracingAccelerationStructureInfo
{
    U64 Location;
};


struct SrvTexture1DInfo
{
    U32 MipLevels;
    U32 MostDetailedMip;
    R32 ResourceMinLODClamp;
};


struct SrvTexture1DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipLevels;
    U32 MostDetailedMip;
    R32 ResourceMinLODClamp;
};


struct SrvTexture2DMSArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
};


struct SrvTexture2DArrayInfo
{
    U32 ArraySize;
    U32 FirstArraySlice;
    U32 MipLevels;
    U32 MostDetailedMip;
    U32 PlaneSlice;
    R32 ResourceMinLODClamp;
};


struct SrvTexture3DInfo
{
    U32 MipLevels;
    U32 MostDetailedMip;
    R32 ResourceMinLODClamp;
};


struct SrvTextureCubeInfo
{
    U32 MipLevels;
    U32 MostDetailedMip;
    R32 ResourceMinLODClamp;
};


struct SrvTextureCubeArrayInfo
{
    U32 First2DArrayFace;
    U32 MipLevels;
    U32 MostDetailedMip;
    U32 NumCubes;
    R32 ResourceMinLODClamp;
};


//! Shader Resource View information.
struct ShaderResourceViewCreateInfo
{
    GPUHandle ResourceHandle;
    PixelFormat Format;
    SRVViewDimension Dimension;
    union 
    {
        SrvBufferInfo Buffer;
        SrvRayTracingAccelerationStructureInfo RaytracingAccelerationStructure;
        SrvTexture1DInfo Texture1D;
        SrvTexture1DArrayInfo Texture1DArray;
        SrvTexture2DInfo Texture2D;
        SrvTexture2DMSArrayInfo Texture2DMSArray;
        SrvTexture2DArrayInfo Texture2DArray;
        SrvTexture3DInfo Texture3D;
        SrvTextureCubeInfo TextureCube;
        SrvTextureCubeArrayInfo TextureCubeArray;
    };
};


//! Unordered Access View information.
struct UnorderedAccessViewCreateInfo
{
    GPUHandle ResourceHandle;
    PixelFormat Format;
    union
    {
    };
};


struct Viewport
{
    R32 X;
    R32 Y;
    R32 Width;
    R32 Height;
    R32 MinDepth;
    R32 MaxDepth;
};


struct Scissor
{
    U32 Left;
    U32 Right;
    U32 Bottom;
    U32 Top;
    U32 Front;
    U32 Back;
};


enum ShaderVisibility
{
    ShaderVisibility_VERTEX =           ( 1 << 0 ),
    ShaderVisibility_PIXEL =            ( 1 << 1 ),
    ShaderVisibility_DOMAIN =           ( 1 << 2 ),
    ShaderVisibility_HULL =             ( 1 << 3 ),
    ShaderVisibility_GEOMETRY =         ( 1 << 4 ),
    ShaderVisibility_ALL =              ( 0xFFFFFFFF )
};


enum DescriptorType
{
    DescriptorType_CONSTANT_BUFFER,
    DescriptorType_SAMPLER,
    DescriptorType_SHADER_RESOURCE_VIEW,
    DescriptorType_UNORDERED_ACCESS_VIEW
};
} // Synthe