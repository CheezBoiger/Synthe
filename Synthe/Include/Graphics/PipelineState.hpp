// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsStructs.hpp"


namespace Synthe {


//! Shader module.
struct ShaderModule
{
    U8* ByteCode;
    U64 SizeInBytes;
};


struct GraphicsPipelineStateCreateInfo
{
    ShaderModule* PVertexShader;
    ShaderModule* PHullShader;
    ShaderModule* PDomainShader;
    ShaderModule* PGeometryShader;
    ShaderModule* PPixelShader;
};


struct ComputePipelineStateCreateInfo
{
    ShaderModule* PComputeShader;
};


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
};
} // Synthe