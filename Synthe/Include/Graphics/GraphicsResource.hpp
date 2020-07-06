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
    DescriptorType_CONSTANT_BUFFER,
    DescriptorType_SAMPLER,
    DescriptorType_SHADER_RESOURCE_VIEW,
    DescriptorType_UNORDERED_ACCESS_VIEW
};


struct ConstantBufferDescriptorInfo
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
    GPUHandle ViewHandle;
    union {
        ConstantBufferDescriptorInfo Buffer;
        ShaderResourceViewDescriptorInfo ShaderResourceView;
        UnorderedAccessViewDescriptorInfo UnorderedAccessView;
        SamplerDescriptorInfo Sampler;
    };
};


//! Create info for descriptor set.
struct DescriptorSetCreateInfo
{
    //! Descriptors to register to our descriptor set.
    DescriptorInfo* PDescriptors;

    //! Number of descriptors to register.
    U32 NumDescriptors;
};


//! Descriptor Set object designed to ease the use of descriptor binding 
//! with low level APIs.
class DescriptorSet
{
public:
    //! Update the descriptor set with new implementations.
    virtual ResultCode Update(const DescriptorSetCreateInfo& Info) { return SResult_NOT_IMPLEMENTED; }

    //! Cleanup the descriptor set.
    virtual ResultCode CleanUp() { return SResult_NOT_IMPLEMENTED; }
};


class Resource
{
public:
};
} // Synthe