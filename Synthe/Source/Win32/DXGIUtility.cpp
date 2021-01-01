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
            return 128u;
        // 64 bits
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            return 64u;

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
            return 32u;
        // Unknown, we assume it is raw buffer, no formatting.
        default:
            return 1u;
    }
}

DXGI_FORMAT GetCommonFormatToDXGIFormat(Synthe::PixelFormat Format)
{
    switch (Format)
    {
        case Synthe::GFormat_R8G8B8A8_UNORM:                return DXGI_FORMAT_R8G8B8A8_UNORM;
        case Synthe::GFormat_R16G16B16A16_FLOAT:            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case Synthe::GFormat_D24_UNORM_S8_UINT:             return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case Synthe::GFormat_D32_FLOAT:                     return DXGI_FORMAT_D32_FLOAT;
        case Synthe::GFormat_R16G16_FLOAT:                  return DXGI_FORMAT_R16G16_FLOAT;
        case Synthe::GFormat_R10G10B10A2_UNORM:             return DXGI_FORMAT_R10G10B10A2_UNORM;
        case Synthe::GFormat_R11G11B10_FLOAT:               return DXGI_FORMAT_R11G11B10_FLOAT;        

        case Synthe::GFormat_UNKNOWN:
        default:                                            return DXGI_FORMAT_UNKNOWN;
    }
}


D3D12_RESOURCE_DIMENSION GetResourceDimension(Synthe::ResourceDimension Dimension)
{
    switch (Dimension)
    {
        case Synthe::ResourceDimension_BUFFER:
            return D3D12_RESOURCE_DIMENSION_BUFFER;
        case Synthe::ResourceDimension_TEXTURE1D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        case Synthe::ResourceDimension_TEXTURE2D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        case Synthe::ResourceDimension_TEXTURE3D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
        default:
            return D3D12_RESOURCE_DIMENSION_UNKNOWN;
    }
}



Synthe::GPUHandle KGPUHandleAssign = 0ULL;


Synthe::GPUHandle GenerateNewHandle()
{
    return ++KGPUHandleAssign; 
}