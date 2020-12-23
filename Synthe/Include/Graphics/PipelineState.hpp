// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsStructs.hpp"
#include "GraphicsResource.hpp"


namespace Synthe {


//! Shader module.
struct ShaderModule
{
    U8* ByteCode;
    U64 SizeInBytes;
};


enum GraphicsFillMode
{
    GraphicsFillMode_SOLID,
    GraphicsFillMode_WIREFRAME
};


enum GraphicsCullMode
{
    GraphicsCullMode_NONE,
    GraphicsCullMode_FRONT,
    GraphicsCullMode_BACK,
};


enum GraphicsFrontFaceWinding
{
    GraphicsFrontFaceWinding_COUNTER_CLOCKWISE,
    GraphicsFrontFaceWinding_CLOCKWISE
};


struct GraphicsRasterStateDesc
{
    GraphicsFillMode            FillMode;
    GraphicsCullMode            CullMode;
    GraphicsFrontFaceWinding    FrontFace;    
};


enum GraphicsDepthWriteMask
{
    GraphicsDepthWriteMask_ZERO,
    GraphicsDepthWriteMask_ALL
};


enum GraphicsBlendStateDesc
{
    
};


enum GraphicsComparison
{
    GraphicsComparison_NEVER,
    GraphicsComparison_LESS,
    GraphicsComparison_EQUAL,
    GraphicsComparison_LESS_EQUAL,
    GraphicsComparison_GREATER,
    GraphicsComparison_GREATER_EQUAL,
    GraphicsComparison_NOT_EQUAL,
    GraphicsComparison_ALWAYS
};


struct GraphicsDepthStencilStateDesc
{
    B32 DepthEnable;
    GraphicsDepthWriteMask DepthWriteMask;
    GraphicsComparison  DepthFunction;
    B32 StencilEnable;
};


//! Graphics pipeline state create info. Fixed function traditional pipeline.
//! 
struct GraphicsPipelineStateCreateInfo
{
    //! Vertex shader module.
    ShaderModule*                   PVertexShader;
    //! Hull shader module.
    ShaderModule*                   PHullShader;
    //! Domain shader module.
    ShaderModule*                   PDomainShader;
    //! Geometry shader module.
    ShaderModule*                   PGeometryShader;
    //! Pixel shader module.
    ShaderModule*                   PPixelShader;
    //! Raster state.
    GraphicsRasterStateDesc         Raster;
    //! Depth stencil state.
    GraphicsDepthStencilStateDesc   DepthStencil;
    GraphicsBlendStateDesc               BlendState;
    RootSignature*                  RootSig;
    U32                             NumRenderTargets;
    U32                             SampleMask;
};


//! Compute pipeline state create info. 
//!
struct ComputePipelineStateCreateInfo
{
    //! Compute shader module.
    ShaderModule* PComputeShader;
};


//! Ray tracing pipeline state create info.
//! This is intended for realtime ray tracing.
struct RayTracingPipelineStateCreateInfo
{
};


struct DescriptorLayoutInfo
{ 
    U32 NumDescriptors;
    U32 BaseRegister;
};


struct DescriptorSetLayoutInfo
{
    DescriptorLayoutInfo Srv;
    DescriptorLayoutInfo Cbv;
    DescriptorLayoutInfo Uav;
    DescriptorLayoutInfo Sampler;
};

// Root Signature is the pipeline layout.
struct RootSignatureLayoutInfo
{   
    U32 NumDescriptorTables;
    DescriptorSetLayoutInfo* LayoutInfos;
};


//! PipelineStateType object.
class PipelineState
{
public:
    virtual ~PipelineState() { }

    //! Get the pipeline state type.    
    PipelineStateType GetType() const { return m_Metadata.Type; }

protected:
    //! Metadata stored for this pipeline state.
    struct {
        DescriptorSetLayoutInfo DescriptorSetLayoutMeta;
        PipelineStateType       Type;    
    } m_Metadata;
};
} // Synthe