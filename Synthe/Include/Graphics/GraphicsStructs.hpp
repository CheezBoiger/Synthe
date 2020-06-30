// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

namespace Synthe {


typedef U64 GPUHandle;
#define GPU_NO_HANDLE 0ULL


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
    GPUVendor_NVIDIA = ( 1 << 0 ),
    //! Vendor Intel.
    GPUVendor_INTEL = ( 1 << 1 ),
    //! Vendor Advanced Micro Devices.
    GPUVendor_AMD = ( 1 << 2 ),
    //! Vendor Qualcomm.
    GPUVendor_QUALCOMM = ( 1 << 3 ),
    //! Vendor Microsoft.
    GPUVendor_MICROSOFT = ( 1 << 4 ),
    //! Any vendor.
    GPUVendor_ANY = 0xFFFFFFFF
};


enum GResult 
{
    GResult_FAILED = -89999,
    GResult_INITIALIZATION_FAILURE,
    GResult_LOST_DEVICE,
    GResult_GPU_MEMORY_CORRUPTION,
    GResult_DEVICE_CREATION_FAILURE,
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
    GFormat_R16G16_FLOAT
};

typedef U32 PixelFormat;

enum GraphicsDeviceFlags
{
    GraphcisDeviceFlags_NONE = 0,
    GraphicsDeviceFlags_SOFTWARE_RASTERIZER_BIT = ( 1 << 0 ),
    GraphicsDeviceFlags_RAYTRACING_HARDWARE_BIT = ( 1 << 1 ),
    GraphicsDeviceFlags_MACHINE_LEARNING_HARWARE_BIT = ( 1 << 2 ),
    GraphicsDeviceFlags_VARIABLE_RATE_SHADING_BIT = ( 1 << 3 ),
    GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT = ( 1 << 4 ),
    GraphicsDeviceFlags_TILED_RESOURCES_BIT = ( 1 << 5 ),
    GraphicsDeviceFlags_BARYCENTRIC_COORDINATES_BIT = ( 1 << 6 )
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
    B32 DesiresRequired : 1;
    //! Enable GPU validation for debugging.
    B32 EnableDeviceDebugLayer : 1;
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
    ResourceUsage_VERTEX_BUFFER         = (1 << 0),
    ResourceUsage_INDEX_BUFFER          = (1 << 1),
    ResourceUsage_UNORDERED_ACCESS      = (1 << 2),
    ResourceUsage_SHADER_RESOURCE       = (1 << 3),
    ResourceUsage_DEPTH_STENCIL         = (1 << 4),
    ResourceUsage_RENDER_TARGET         = (1 << 5)
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


} // Synthe