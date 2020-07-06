// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12DescriptorManager.hpp"
#include "D3D12Resource.hpp"
#include "D3D12GraphicsDevice.hpp"


namespace Synthe {


ResultCode D3D12DescriptorSet::Update(const DescriptorSetCreateInfo& Info)
{
    DescriptorPool* CBVSRVUAVPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV);
    DescriptorPool* SamplerPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER);

    U64 CBVSRVUAVAlignment = CBVSRVUAVPool->GetAlignmentSizeInBytes();
    U64 SamplerAlignment = SamplerPool->GetAlignmentSizeInBytes();

    std::vector<DescriptorInfo> CBVSRVUAVInfos;
    std::vector<DescriptorInfo> SamplerInfos;

    for (U32 I = 0; I < Info.NumDescriptors; ++I)
    {
        DescriptorInfo& Descriptor = Info.PDescriptors[I];
        switch (Descriptor.Type)
        {
            case DescriptorType_SHADER_RESOURCE_VIEW:
            case DescriptorType_CONSTANT_BUFFER:
            case DescriptorType_UNORDERED_ACCESS_VIEW:
            {
                CBVSRVUAVInfos.push_back(Descriptor);
                break;
            }
            case DescriptorType_SAMPLER:
            {
                SamplerInfos.push_back(Descriptor);
                break;
            }
            default:
                break;
        }
    }

    CBVSRVUAVPool->AllocateDescriptorTable(&m_Table, CBVSRVUAVInfos.size() * CBVSRVUAVAlignment);
    SamplerPool->AllocateDescriptorTable(&m_SamplerTable, SamplerInfos.size() * SamplerAlignment);

    for (U32 I = 0; I < CBVSRVUAVInfos.size(); ++I)
    {
        DescriptorInfo& Descriptor = CBVSRVUAVInfos[I];
        switch (Descriptor.Type)
        {
            case DescriptorType_SHADER_RESOURCE_VIEW:
            case DescriptorType_CONSTANT_BUFFER:
            case DescriptorType_UNORDERED_ACCESS_VIEW:
                break;
        }
    }

    for (U32 I = 0; I < SamplerInfos.size(); ++I)
    {
        DescriptorInfo& Descriptor = SamplerInfos[I];
        switch (Descriptor.Type)
        {
            case DescriptorType_SAMPLER:
                break;
        }
    }
    return SResult_OK;
}
} // Synthe