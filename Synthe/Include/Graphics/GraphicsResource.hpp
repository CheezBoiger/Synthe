// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsBuffer.hpp"
#include "GraphicsResourceView.hpp"
#include "GraphicsStructs.hpp"

namespace Synthe {


//! Descriptor Info is the information of the resource bound to the 
//! descriptor set that is mapped by the graphics device. This is similar to 
//! Vulkan implementation.
struct DescriptorInfo 
{
    DescriptorType Type;
    U32 Binding;
    GPUHandle ViewHandle;
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
    virtual ~DescriptorSet() { }

    //! Update the descriptor set with new implementations.
    virtual ResultCode Update(const DescriptorSetCreateInfo& Info) { return SResult_NOT_IMPLEMENTED; }

    //! Cleanup the descriptor set.
    virtual ResultCode CleanUp() { return SResult_NOT_IMPLEMENTED; }
};


class Resource
{
public:
};


class RootSignature
{
public:
    virtual ~RootSignature() { }

    
private:
};
} // Synthe