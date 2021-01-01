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
    
    //! Generate native blend description.
    static D3D12_BLEND_DESC GenerateBlendDescription(const GraphicsBlendStateDesc& Info);

    //! Generate input layout description.
    static D3D12_INPUT_LAYOUT_DESC GenerateInputLayoutDescription();

    //! Initialize the native graphics pipeline state.
    ResultCode Initialize(ID3D12Device* PDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc);
    //! Initialize the native compute pipeline state.
    ResultCode Initialize(ID3D12Device* PDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC& Desc);

    //! Return the native pipeline state handled by the driver.
    ID3D12PipelineState* GetNative() { return m_Pipeline; }

private:
    //! Native pipeline.
    ID3D12PipelineState* m_Pipeline;
};
} // Synthe