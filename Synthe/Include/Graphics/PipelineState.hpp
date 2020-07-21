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
    
};


struct ComputePipelineStateCreateInfo
{
};


struct RayTracingPipelineStateCreateInfo
{
};


struct DescriptorLayoutInfo
{ 
    U32 NumDescriptors;
    U32 BaseRegister;
};


struct DescriptorTableLayoutInfo
{
    DescriptorLayoutInfo Srv;
    DescriptorLayoutInfo Cbv;
    DescriptorLayoutInfo Uav;
    DescriptorLayoutInfo Sampler;
};

// Root Signature is the pipeline layout.
struct RootSignatureCreateInfo
{   
    U32 NumDescriptorTables;
    DescriptorTableLayoutInfo* LayoutInfos;
};


//! PipelineStateType object.
class PipelineState
{
public:
    virtual ~PipelineState() { }
};
} // Synthe