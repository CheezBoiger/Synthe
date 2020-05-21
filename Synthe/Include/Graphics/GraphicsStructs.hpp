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

enum GResult 
{
    GResult_OK = 1,
    GResult_FAILED = -9999,
    GResult_INITIALIZATION_FAILURE,
    GResult_MEMORY_CORRUPTION,
    GResult_LOST_DEVICE,
    GResult_CREATION_FAILTURE,
    GResult_INVALID_CALL,
    GResult_INVALID_ARGS,
    GResult_UNKNOWN
};


enum GPUVendor
{
    GPUVendor_UNKNOWN,
    GPUVendor_NVIDIA = (1<<0),
    GPUVendor_INTEL = (1<<1),
    GPUVendor_AMD = (1<<2),
    GPUVendor_QUALCOMM = (1<<3),
    GPUVendor_MICROSOFT = (1<<4),
    GPUVendor_ANY = 0xFFFFFFFF
};

enum SwapchainFlags
{
    SwapchainFlags_NONE,
    SwapchainFlags_ALLOW_TEARING
};


struct SwapchainConfig
{
    U32 Width;
    U32 Height;
    U32 Count;
    U32 Format;
    U64 NativeWinHandle;
    U32 Windowed;
    SwapchainFlags Flags;
};


enum GFormat
{
    GFormat_R8G8B8A8_UNORM,
    GFormat_R16G16B16A16_FLOAT,
    GFormat_R16G16_FLOAT,
};



enum GraphicsDeviceFlags
{
    GraphcisDeviceFlags_NONE = 0,
    GraphicsDeviceFlags_SOFTWARE_RASTERIZER_BIT = (1<<0),
    GraphicsDeviceFlags_RAYTRACING_HARDWARE_BIT = (1<<1),
    GraphicsDeviceFlags_MACHINE_LEARNING_HARWARE_BIT = (1<<2),
    GraphicsDeviceFlags_VARIABLE_RATE_SHADING_BIT = (1<<3),
    GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT = (1<<4),
    GraphicsDeviceFlags_TILED_RESOURCES_BIT = (1<<5),
    GraphicsDeviceFlags_BARYCENTRIC_COORDINATES_BIT = (1<<6)
};


struct GraphicsDeviceConfig
{
    // Desired GPU Vendor.
    GPUVendor DesiredVendor;
    // Desired Flags.
    GraphicsDeviceFlags DesiredFlags;
    // True if the specified desires are required.
    B32 DesiresRequired : 1;
    // Enable GPU validation for debugging.
    B32 EnableDeviceDebugLayer : 1;
};


typedef U64 GPUHandle;
} // Synthe