// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsBuffer.hpp"
#include "GraphicsResource.hpp"
#include "GraphicsResourceView.hpp"


namespace Synthe {


enum DescriptorType
{
    DescriptorType_BUFFER,
    DescriptorType_SAMPLER,
    DescriptorType_SHADER_RESOURCE_VIEW,
    DescriptorType_UNORDERED_ACCESS_VIEW
};


struct BufferDescriptorInfo
{
};


struct ShaderResourceViewDescriptorInfo 
{
};


struct UnorderedAccessViewDescriptorInfo
{
};

struct SamplerDescriptorInfo
{
};

//! Descriptor Info is the information of the resource bound to the 
//! descriptor set that is mapped by the graphics device. This is similar to 
//! Vulkan implementation.
struct DescriptorInfo 
{
    DescriptorType Type;
    U32 Binding;
    union {
        BufferDescriptorInfo Buffer;
        ShaderResourceViewDescriptorInfo ShaderResourceView;
        UnorderedAccessViewDescriptorInfo UnorderedAccessView;
        SamplerDescriptorInfo Sampler;
    };
};


struct DescriptorSetCreateInfo
{
    DescriptorInfo* PDescriptors;
    U32 NumDescriptors;
};


class DescriptorSet
{
public:
    //! Update the descriptor set with new implementations.
    virtual ResultCode Update(DescriptorSetCreateInfo* Info) { return SResult_NOT_IMPLEMENTED; }

    //! Cleanup the descriptor set.
    virtual ResultCode CleanUp() { return SResult_NOT_IMPLEMENTED; }
};


class Resource
{
public:
};
} // Synthe