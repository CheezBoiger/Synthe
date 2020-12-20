// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "Common/Types.hpp"

#include "D3D12ResourceView.hpp"
#include "Graphics/GraphicsResourceView.hpp"
#include "Graphics/GraphicsStructs.hpp"


namespace Synthe {


ResultCode GetSrvDescription(const ShaderResourceViewCreateInfo& SRV, D3D12_SHADER_RESOURCE_VIEW_DESC& Desc)
{
    Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    Desc.Format = GetCommonFormatToDXGIFormat(SRV.Format);

    switch (SRV.Dimension)
    {
        case SrvViewDimension_BUFFER:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            Desc.Buffer.FirstElement = SRV.Buffer.FirstElement;
            Desc.Buffer.Flags = (D3D12_BUFFER_SRV_FLAGS)SRV.Buffer.Flags;
            Desc.Buffer.NumElements = SRV.Buffer.NumElements;
            Desc.Buffer.StructureByteStride = SRV.Buffer.StructureByteStride;
            break;
        }
        case SrvViewDimension_RAYTRACING_ACCELERATION_STRUCTURE:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
            Desc.RaytracingAccelerationStructure.Location = SRV.RaytracingAccelerationStructure.Location;
            break;
        }
        case SrvViewDimension_TEXTURE1D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipLevels = SRV.Texture1D.MipLevels;
            Desc.Texture1D.MostDetailedMip = SRV.Texture1D.MostDetailedMip;
            Desc.Texture1D.ResourceMinLODClamp = SRV.Texture1D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE1D_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = SRV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = SRV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipLevels = SRV.Texture1DArray.MipLevels;
            Desc.Texture1DArray.MostDetailedMip = SRV.Texture1DArray.MostDetailedMip;
            Desc.Texture1DArray.ResourceMinLODClamp = SRV.Texture1DArray.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE2D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipLevels = SRV.Texture2D.MipLevels;
            Desc.Texture2D.MostDetailedMip = SRV.Texture2D.MostDetailedMip;
            Desc.Texture2D.PlaneSlice = SRV.Texture2D.PlaneSlice;
            Desc.Texture2D.ResourceMinLODClamp = SRV.Texture2D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE2DMS:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        }
        case SrvViewDimension_TEXTURE2DMS_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize = SRV.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice = SRV.Texture2DMSArray.FirstArraySlice;
            break;
        }
        case SrvViewDimension_TEXTURE2D_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = SRV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = SRV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipLevels = SRV.Texture2DArray.MipLevels;
            Desc.Texture2DArray.MostDetailedMip = SRV.Texture2DArray.MostDetailedMip;
            Desc.Texture2DArray.PlaneSlice = SRV.Texture2DArray.PlaneSlice;
            Desc.Texture2DArray.ResourceMinLODClamp = SRV.Texture2DArray.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE3D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
            Desc.Texture3D.MipLevels = SRV.Texture3D.MipLevels;
            Desc.Texture3D.MostDetailedMip = SRV.Texture3D.MostDetailedMip;
            Desc.Texture3D.ResourceMinLODClamp = SRV.Texture3D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE_CUBE:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
            Desc.TextureCube.MipLevels = SRV.TextureCube.MipLevels;
            Desc.TextureCube.MostDetailedMip = SRV.TextureCube.MostDetailedMip;
            Desc.TextureCube.ResourceMinLODClamp = SRV.TextureCube.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE_CUBE_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
            Desc.TextureCubeArray.First2DArrayFace = SRV.TextureCubeArray.First2DArrayFace;
            Desc.TextureCubeArray.MipLevels = SRV.TextureCubeArray.MipLevels;
            Desc.TextureCubeArray.MostDetailedMip = SRV.TextureCubeArray.MostDetailedMip;
            Desc.TextureCubeArray.NumCubes = SRV.TextureCubeArray.NumCubes;
            Desc.TextureCubeArray.ResourceMinLODClamp = SRV.TextureCubeArray.ResourceMinLODClamp;
            break;
        }
    }
    
    return SResult_OK;
}


ResultCode GetRtvDescription(const RenderTargetViewCreateInfo& RTV, D3D12_RENDER_TARGET_VIEW_DESC& Desc)
{
    Desc.Format = GetCommonFormatToDXGIFormat(RTV.Format);
    switch (RTV.Dimension)
    {
        case RTVViewDimension_BUFFER:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_BUFFER;
            Desc.Buffer.FirstElement = RTV.Buffer.FirstElement;
            Desc.Buffer.NumElements = RTV.Buffer.NumElements;
            break;
        case RTVViewDimension_TEXTURE_1D:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipSlice = RTV.Texture1D.MipSlice;
            break;
        case RTVViewDimension_TEXTURE_2D:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipSlice = RTV.Texture2D.MipSlice;
            Desc.Texture2D.PlaneSlice = RTV.Texture2D.PlaneSlice;
            break;
        case RTVViewDimension_TEXTURE_1D_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = RTV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = RTV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipSlice = RTV.Texture1DArray.MipSlice;
            break;
        case RTVViewDimension_TEXTURE_2D_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = RTV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = RTV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipSlice = RTV.Texture2DArray.MipSlice;
            Desc.Texture2DArray.PlaneSlice = RTV.Texture2DArray.PlaneSlice;
            break;
        case RTVViewDimension_TEXTURE_2DMS_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice;
            break;
        case RTVViewDimension_TEXTURE_2DMS:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        case RTVViewDimension_TEXTURE_3D:
            Desc.Texture3D.FirstWSlice = RTV.Texture3D.FirstWSlice;
            Desc.Texture3D.MipSlice = RTV.Texture3D.MipSlice;
            Desc.Texture3D.WSize = RTV.Texture3D.WSize;
            break;
        default:
            // No proper dimension was set.
            return SResult_INVALID_ARGS;
    }

    return SResult_OK;
}


ResultCode GetDsvDescription(const DepthStencilViewCreateInfo& DSV, D3D12_DEPTH_STENCIL_VIEW_DESC& Desc)
{
    Desc.Format = GetCommonFormatToDXGIFormat(DSV.Format);
    switch (DSV.Dimension)
    {
        case DSVViewDimension_TEXTURE1D:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipSlice = DSV.Texture1D.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE1D_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = DSV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = DSV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipSlice = DSV.Texture1DArray.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE2D:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipSlice = DSV.Texture2D.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE2DMS:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        }
        case DSVViewDimension_TEXTURE2DMS_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize = DSV.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice = DSV.Texture2DMSArray.FirstArraySlice;
            break;
        }
        case DSVViewDimension_TEXTURE2D_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = DSV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = DSV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipSlice = DSV.Texture2DArray.MipSlice;
            break;
        }
    }

    return SResult_OK;
}
} // Synthe