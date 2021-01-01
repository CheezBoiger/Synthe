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
    switch (Info.CullMode)
    {
        case GraphicsCullMode_NONE:                         Desc.CullMode = D3D12_CULL_MODE_NONE; break;
        case GraphicsCullMode_FRONT:                        Desc.CullMode = D3D12_CULL_MODE_FRONT; break;
        case GraphicsCullMode_BACK:                         Desc.CullMode = D3D12_CULL_MODE_BACK; break;
        default: break;
    }
    
    switch (Info.FillMode)
    {
        case GraphicsFillMode_WIREFRAME:                    Desc.FillMode = D3D12_FILL_MODE_WIREFRAME; break;
        case GraphicsFillMode_SOLID:                        Desc.FillMode = D3D12_FILL_MODE_SOLID; break;
        default: break;
    }

    switch (Info.FrontFace)
    {
        case GraphicsFrontFaceWinding_CLOCKWISE:            Desc.FrontCounterClockwise = false; break;
        case GraphicsFrontFaceWinding_COUNTER_CLOCKWISE:    Desc.FrontCounterClockwise = true; break;
        default: break;
    }

    return Desc;
}


D3D12_BLEND_DESC D3D12PipelineState::GenerateBlendDescription(const GraphicsBlendStateDesc& Info)
{
    D3D12_BLEND_DESC Desc = { };
    Desc.AlphaToCoverageEnable;
    Desc.IndependentBlendEnable;
    for (U32 I = 0; I < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++I)
    {
        Desc.RenderTarget[I].BlendEnable;
        Desc.RenderTarget[I].BlendOp;
        Desc.RenderTarget[I].BlendOpAlpha;
        Desc.RenderTarget[I].DestBlend;
        Desc.RenderTarget[I].DestBlendAlpha;
        Desc.RenderTarget[I].LogicOp;
        Desc.RenderTarget[I].LogicOpEnable;
        Desc.RenderTarget[I].RenderTargetWriteMask;
        Desc.RenderTarget[I].SrcBlend;
        Desc.RenderTarget[I].SrcBlendAlpha;
    }
    return Desc;
}


D3D12_INPUT_LAYOUT_DESC D3D12PipelineState::GenerateInputLayoutDescription()
{
    D3D12_INPUT_LAYOUT_DESC Desc = { };

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


ResultCode D3D12PipelineState::Initialize(ID3D12Device* PDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC& Desc)
{
    HRESULT Result = PDevice->CreateComputePipelineState(&Desc, __uuidof(ID3D12PipelineState), (void**)m_Pipeline);
    if (FAILED(Result))
    {
        return SResult_INITIALIZATION_FAILURE;
    }

    m_Metadata.Type = PipelineStateType_COMPUTE;
    return SResult_OK;
}
} // Synthe