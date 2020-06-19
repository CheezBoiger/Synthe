// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

namespace Synthe {


enum GraphicsAPI
{
    GRAPHICS_API_DIRECT3D12,
    GRAPHICS_API_DIRECT3D11
};


enum GPUVendor
{
    GPUVendor_UNKNOWN,
    GPUVendor_NVIDIA = ( 1 << 0 ),
    GPUVendor_INTEL = ( 1 << 1 ),
    GPUVendor_AMD = ( 1 << 2 ),
    GPUVendor_QUALCOMM = ( 1 << 3 ),
    GPUVendor_MICROSOFT = ( 1 << 4 ),
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
};


enum GFormat
{
    GFormat_R8G8B8A8_UNORM,
    GFormat_R16G16B16A16_FLOAT,
    GFormat_R16G16_FLOAT
};

typedef U32 Format;

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


struct InstancedKey
{
    
};


typedef U64 GPUHandle;
} // Synthe