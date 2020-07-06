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


struct RootSignatureCreateInfo
{
    
};
} // Synthe