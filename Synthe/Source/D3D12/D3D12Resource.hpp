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
    ResultCode Update(const DescriptorSetCreateInfo& Info) override;

    //! 
    ResultCode CleanUp() override;

    //! Set the new table of the descriptorset, this updates where the 
    //! descriptor set's resources are located in the descriptor heap.
    //!
    //! \param Table
    void SetNewTable(DescriptorTable Table) { m_TableUpload = Table; }
    
    //! Get the current descriptor table from this descriptor set.
    //! As the application flips to working buffers, these descriptor
    //! sets will be updated constantly.
    DescriptorTable GetTable() const { return m_GPUTable; }

    //! Get Sampler table.
    DescriptorTable GetSamplerTable() const { return m_GPUSamplerTable; }

    //! Upload CPU descriptor heap to GPU visible descriptor heap.
    //!
    //! \param PDevice
    //! \param CurrentBufferIndex
    void UploadToShaderVisibleHeap(ID3D12Device* PDevice, U32 CurrentBufferIndex);

private:
    //! Table that contains the offsets of our resources.
    DescriptorTable m_TableUpload;

    //!
    DescriptorTable m_SamplerTableUpload;

    //!
    DescriptorTable m_GPUTable;

    //!
    DescriptorTable m_GPUSamplerTable;   
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