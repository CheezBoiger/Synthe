// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12DescriptorManager.hpp"
#include "D3D12Resource.hpp"
#include "D3D12GraphicsDevice.hpp"


namespace Synthe {


ResultCode D3D12DescriptorSet::Update(const DescriptorSetUpdateInfo& Info)
{
    ID3D12Device* PDevice = static_cast<D3D12GraphicsDevice*>(GetDeviceD3D12())->GetNative();
    DescriptorPool* CBVSRVUAVPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV);
    DescriptorPool* SamplerPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER);
    U64 CBVSRVUAVAlignment = CBVSRVUAVPool->GetAlignmentSizeInBytes();
    U64 SamplerAlignment = SamplerPool->GetAlignmentSizeInBytes();
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> CBVInfos;
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> SRVInfos;
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> UAVInfos;
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> SamplerInfos;

    for (U32 I = 0; I < Info.NumDescriptors; ++I)
    {
        DescriptorInfo& Descriptor = Info.PDescriptors[I];
        switch (Descriptor.Type)
        {
            case DescriptorType_SHADER_RESOURCE_VIEW:
            {
                SRVInfos.reserve(Descriptor.Binding + 1ULL);
                SRVInfos[Descriptor.Binding] = {Descriptor.ViewHandle};
                break;
            }
            case DescriptorType_CONSTANT_BUFFER:
            {
                CBVInfos.reserve(Descriptor.Binding + 1ULL);
                CBVInfos[Descriptor.Binding] = {Descriptor.ViewHandle};
                break;
            }
            case DescriptorType_UNORDERED_ACCESS_VIEW:
            {
                UAVInfos.reserve(Descriptor.Binding + 1ULL);
                UAVInfos[Descriptor.Binding] = {Descriptor.ViewHandle};
                break;
            }
            case DescriptorType_SAMPLER:
            {
                SamplerInfos.reserve(Descriptor.Binding + 1ULL);
                SamplerInfos[Descriptor.Binding] = {Descriptor.ViewHandle};
                break;
            }
            default:
            {
                break;
            }
        }
    }

    {
        D3D12_CPU_DESCRIPTOR_HANDLE OffsetAddress = TableUpload.StartingAddress;
        CBVSRVUAVPool->CopyDescriptorsRange(PDevice, SRVInfos.size(), SRVInfos.data(), OffsetAddress);
        OffsetAddress.ptr += SRVInfos.size() + CBVSRVUAVAlignment;
        CBVSRVUAVPool->CopyDescriptorsRange(PDevice, CBVInfos.size(), CBVInfos.data(), OffsetAddress);
        OffsetAddress.ptr += CBVInfos.size() + CBVSRVUAVAlignment;
        CBVSRVUAVPool->CopyDescriptorsRange(PDevice, UAVInfos.size(), UAVInfos.data(), OffsetAddress);
    }

    {
        SamplerPool->CopyDescriptorsRange(PDevice, SamplerInfos.size(), SamplerInfos.data(), 
            SamplerTableUpload.StartingAddress);
        m_NeedsFlushToGPU = true;
    }
    return SResult_OK;
}


void D3D12DescriptorSet::UploadToShaderVisibleHeap(ID3D12Device* PDevice, U32 CurrentBufferIndex)
{
    if (NeedsToBeFlushed()) 
    {
        DescriptorPool* Pool = 
            D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV, CurrentBufferIndex);
        DescriptorPool* SamplerPool =
            D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER, CurrentBufferIndex);

        Pool->CopyDescriptorsRangeConsecutive(PDevice, TableUpload.StartingAddress, 
            TableUpload.TableSizeInBytes / Pool->GetAlignmentSizeInBytes(), 
            GPUTable.StartingAddress);

        SamplerPool->CopyDescriptorsRangeConsecutive(PDevice, SamplerTableUpload.StartingAddress,
            SamplerTableUpload.TableSizeInBytes / SamplerPool->GetAlignmentSizeInBytes(), 
            GPUSamplerTable.StartingAddress);
    }

    m_NeedsFlushToGPU = false;
}


ResultCode D3D12DescriptorSet::Release()
{
    DescriptorPool* Pool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV);
    
    return SResult_OK;
}
} // Synthe