// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "Win32Common.hpp"


UINT GetBitsForPixelFormat(DXGI_FORMAT Format)
{
    switch (Format)
    {
        // 128 bits
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_UINT:
            return 128;
        // 64 bits
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            return 64;

        // 32 bits.
        case DXGI_FORMAT_R11G11B10_FLOAT:

        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:

        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:

        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_TYPELESS:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_UNORM:

        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_R32_UINT:

        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:

        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return 32;
        // Unknown, we assume it is raw buffer, no formatting.
        default:
            return 0;
    }
}