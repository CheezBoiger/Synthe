// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/GraphicsResource.hpp"
#include "Graphics/GraphicsStructs.hpp"

#include "D3D12DescriptorManager.hpp"
#include "D3D12MemoryManager.hpp"


namespace Synthe {


class D3D12DescriptorSet : public DescriptorSet
{
public:
    //! Update descriptor set. This updates CPU bound descriptors, so be sure to 
    //! call UploadToShaderVisibleHeap, in order to keep this object coherent.
    ResultCode Update(const DescriptorSetUpdateInfo& Info) override;

    //! Clean up this descriptor set, release on finish.
    ResultCode Release() override;

    //! Upload CPU descriptor heap to GPU visible descriptor heap.
    //!
    //! \param PDevice
    //! \param CurrentBufferIndex
    void UploadToShaderVisibleHeap(ID3D12Device* PDevice, U32 CurrentBufferIndex);

    //!
    B32 NeedsToBeFlushed() const { return m_NeedsFlushToGPU; }

    //! Table that contains the offsets of our resources.
    DescriptorTable TableUpload;

    //! Sampler table that contains the offsets of our resources.
    DescriptorTable SamplerTableUpload;

    //! GPU table used for input in shaders.
    DescriptorTable GPUTable;

    //! Sampler table used for input in shaders.
    DescriptorTable GPUSamplerTable;   

private:

    //! Bool to check if this descriptor set needs to actually be flushed to the shader visible set.
    //! Default set to false, until Update() is called.
    B32 m_NeedsFlushToGPU;
};


class D3D12RootSignature : public RootSignature
{
public:
    D3D12RootSignature(ID3D12RootSignature* PRoot) 
        : m_RootSignature(PRoot) { }

    ID3D12RootSignature* GetSignature() { return m_RootSignature; }
private:
    ID3D12RootSignature* m_RootSignature;
};


class D3D12Resource : public Resource
{
public:
    
};
} // Synthe