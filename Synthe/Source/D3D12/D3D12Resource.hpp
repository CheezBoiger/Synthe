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
    //! 
    ResultCode Update(const DescriptorSetUpdateInfo& Info) override;

    //! 
    ResultCode CleanUp() override;

    //! Upload CPU descriptor heap to GPU visible descriptor heap.
    //!
    //! \param PDevice
    //! \param CurrentBufferIndex
    void UploadToShaderVisibleHeap(ID3D12Device* PDevice, U32 CurrentBufferIndex);

    //! Table that contains the offsets of our resources.
    DescriptorTable TableUpload;

    //!
    DescriptorTable SamplerTableUpload;

    //!
    DescriptorTable GPUTable;

    //!
    DescriptorTable GPUSamplerTable;   
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
} // Synthe