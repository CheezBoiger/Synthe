// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once

#include "Win32Common.hpp"
#include "Graphics/PipelineState.hpp"

namespace Synthe {

class D3D12PipelineState : public PipelineState
{
public:
    D3D12PipelineState()
        : m_Pipeline(nullptr) { }

    //! Generate the native depth stencil description.
    static D3D12_DEPTH_STENCIL_DESC GenerateDepthStencilDescription(const GraphicsDepthStencilStateDesc& Info);

    //! Generate the native rasterizer description.
    static D3D12_RASTERIZER_DESC GenerateRasterizerDescription(const GraphicsRasterStateDesc& Info);
    
    //!
    static D3D12_BLEND_DESC GenerateBlendDescription(const GraphicsBlendStateDesc& Info);

    //! Initialize the native graphics pipeline state.
    ResultCode Initialize(ID3D12Device* PDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc);
    ResultCode Initialize(ID3D12Device* PDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC& Desc);

private:
    //! Native pipeline.
    ID3D12PipelineState* m_Pipeline;
};
} // Synthe