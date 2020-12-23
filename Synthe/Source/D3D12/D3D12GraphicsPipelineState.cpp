// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GraphicsPipelineState.hpp"


namespace Synthe {


D3D12_DEPTH_STENCIL_DESC D3D12PipelineState::GenerateDepthStencilDescription(const GraphicsDepthStencilStateDesc& Info)
{
    D3D12_DEPTH_STENCIL_DESC Desc = { };

    Desc.DepthEnable = Info.DepthEnable;

    switch (Info.DepthFunction)
    {   
        case GraphicsComparison_ALWAYS:         Desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; break;
        case GraphicsComparison_EQUAL:          Desc.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL; break;
        case GraphicsComparison_GREATER:        Desc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER; break;
        case GraphicsComparison_LESS:           Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS; break;
        case GraphicsComparison_LESS_EQUAL:     Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; break;
        case GraphicsComparison_NEVER:          Desc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER; break;
        case GraphicsComparison_NOT_EQUAL:      Desc.DepthFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL; break;
        case GraphicsComparison_GREATER_EQUAL:  Desc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL; break;
        default: break;
    }

    switch (Info.DepthWriteMask)
    {
        case GraphicsDepthWriteMask_ALL:        Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; break; 
        case GraphicsDepthWriteMask_ZERO:       Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; break;
        default: break;
    }

    return Desc;
}


D3D12_RASTERIZER_DESC D3D12PipelineState::GenerateRasterizerDescription(const GraphicsRasterStateDesc& Info)
{
    D3D12_RASTERIZER_DESC Desc = { };
    
    return Desc;
}


D3D12_BLEND_DESC D3D12PipelineState::GenerateBlendDescription(const GraphicsBlendStateDesc& Info)
{
    D3D12_BLEND_DESC Desc = { };
    Desc.AlphaToCoverageEnable;
    Desc.IndependentBlendEnable;
    for (U32 I = 0; I < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++I)
    {
        
    }
    return Desc;
}


ResultCode D3D12PipelineState::Initialize(ID3D12Device* PDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc)
{
    HRESULT Result = PDevice->CreateGraphicsPipelineState(&Desc, __uuidof(ID3D12PipelineState), (void**)&m_Pipeline);

    if (FAILED(Result))
    {
        return SResult_INITIALIZATION_FAILURE;
    }

    m_Metadata.Type = PipelineStateType_GRAPHICS;
    return SResult_OK;
}
} // Synthe