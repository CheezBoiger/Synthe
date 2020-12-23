// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12CommandList.hpp"
#include "D3D12GraphicsDevice.hpp"
#include "D3D12DescriptorManager.hpp"
#include "D3D12MemoryManager.hpp"
#include "D3D12Resource.hpp"


namespace Synthe {


ResultCode D3D12GraphicsCommandList::Initialize(ID3D12Device* PDevice, 
                                                U32 NumCommandListBuffers,
                                                ID3D12CommandAllocator** CommandAllocators,
                                                D3D12_COMMAND_LIST_TYPE Type)
{
    m_DeviceRef = PDevice;
    Release();

    m_CommandLists.resize(NumCommandListBuffers);
    for (U32 Idx = 0; Idx < m_CommandLists.size(); ++Idx)
    {
        HRESULT Result = PDevice->CreateCommandList(0, Type, CommandAllocators[Idx], nullptr, 
                            __uuidof(ID3D12GraphicsCommandList5), (void**)&m_CommandLists[Idx].PCmdList);
        if (FAILED(Result))
        {
            return SResult_FAILED;
        }
        m_CommandLists[Idx].PCmdList->Close();
        m_CommandLists[Idx].State = CommandState_READY;
        m_CommandLists[Idx].PAllocatorRef = CommandAllocators[Idx];
    }
    return SResult_OK;
}


void D3D12GraphicsCommandList::Release()
{
    for (CommandListState& State : m_CommandLists)
    {
        State.PCmdList->Release();       
    }
    m_CommandLists.clear();
}


void D3D12GraphicsCommandList::End()
{
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->Close();
    m_CommandLists[m_CurrentRecordingIdx].State = CommandState_READY;
}


void D3D12GraphicsCommandList::Begin()
{
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->Reset(
        m_CommandLists[m_CurrentRecordingIdx].PAllocatorRef, nullptr);
    m_CommandLists[m_CurrentRecordingIdx].State = CommandState_STILL_RECORDING;
}


void D3D12GraphicsCommandList::SetRenderTargets(U32 NumRTVs, GPUHandle* RTVHandles, GPUHandle* DepthStencil)
{
    static D3D12_CPU_DESCRIPTOR_HANDLE RTVBuffers[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
    D3D12_CPU_DESCRIPTOR_HANDLE DSVBuffer = { 0 };
    for (U32 I = 0; I < NumRTVs; ++I)
    {
        RTVBuffers[I].ptr = RTVHandles[I];
    }
    if (DepthStencil)
    {
        DSVBuffer.ptr = *DepthStencil;
    }
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->OMSetRenderTargets(
        NumRTVs, RTVBuffers, FALSE, DepthStencil ? &DSVBuffer : nullptr);
}


void D3D12GraphicsCommandList::SetViewports(U32 NumViewports, const Viewport* PViewports)
{
    D3D12_VIEWPORT Viewports[8];
    for (U32 I = 0; I < NumViewports; ++I)
    {
        Viewports[I].Height = PViewports[I].Height;
        Viewports[I].Width = PViewports[I].Width;
        Viewports[I].TopLeftX = PViewports[I].X;
        Viewports[I].TopLeftY = PViewports[I].Y;
        Viewports[I].MinDepth = PViewports[I].MinDepth;
        Viewports[I].MaxDepth = PViewports[I].MaxDepth;
    }
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->RSSetViewports(NumViewports, Viewports);
}


void D3D12GraphicsCommandList::SetScissors(U32 NumScissors, const Scissor* PScissors)
{
    D3D12_RECT Scissors[8];
    for (U32 I = 0; I < NumScissors; ++I)
    {
        Scissors[I].left = PScissors[I].Left;
        Scissors[I].right = PScissors[I].Right;
        Scissors[I].top = PScissors[I].Top;
        Scissors[I].bottom = PScissors[I].Bottom;
    }
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->RSSetScissorRects(NumScissors, Scissors);
}


void D3D12GraphicsCommandList::ClearRenderTarget(GPUHandle RTV, 
                                                 ClearColorValue* ClearColor, 
                                                 U32 NumBounds, 
                                                 TargetBounds* Bounds)
{
    static float RGBA[4];
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = { RTV };
    D3D12_RESOURCE_STATES State = D3D12_RESOURCE_STATE_RENDER_TARGET;
    TransitionResourceIfNeeded(1, &RTV, &State);
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->ClearRenderTargetView(Handle, 
        (float*)ClearColor, NumBounds, (D3D12_RECT*)Bounds);
}


void D3D12GraphicsCommandList::TransitionResourceIfNeeded(U32 NumHandles, GPUHandle* Descriptors, D3D12_RESOURCE_STATES* NeededStates)
{
    static D3D12_RESOURCE_BARRIER Barriers[64];
    for (U32 I = 0; I < NumHandles; ++I)
    {
        ResourceState Resource;
        GPUHandle Key;
        D3D12DescriptorManager::GetCachedResourceWithDescriptor(Descriptors[I], &Key);
        D3D12MemoryManager::GetNativeResource(Key, &Resource);
        if (Resource.State == NeededStates[I])
        {
            return;
        }
        Barriers[I].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        Barriers[I].Transition.pResource = Resource.PResource;
        Barriers[I].Transition.StateAfter = NeededStates[I];
        Barriers[I].Transition.StateBefore = Resource.State;
        Barriers[I].Transition.Subresource = 0;
        D3D12MemoryManager::UpdateResourceState(Key, NeededStates[I]);
    }
    m_CommandLists[m_CurrentRecordingIdx].PCmdList->ResourceBarrier(NumHandles, Barriers);
}


void D3D12GraphicsCommandList::BindDescriptorSets(U32 NumSets, DescriptorSet* const* PDescriptorSets)
{
    for (U32 I = 0; I < NumSets; ++I)
    {
        const D3D12DescriptorSet* Set = static_cast<const D3D12DescriptorSet*>(PDescriptorSets[I]);
        D3D12_GPU_DESCRIPTOR_HANDLE DescriptorTableGPUAddress = 
            D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV_UPLOAD)->GetGPUAddressFromCPUAddress(
                Set->GPUTable.StartingAddress);
        
        m_CommandLists[m_CurrentRecordingIdx].PCmdList->SetGraphicsRootDescriptorTable(
            I, DescriptorTableGPUAddress);
    }
}


void D3D12GraphicsCommandList::SetPipelineState(PipelineStateType PipelineType,
                                                PipelineState* PPipelineState,
                                                RootSignature* PRootSignature)
{ 
    ID3D12GraphicsCommandList* CommandList = m_CommandLists[m_CurrentRecordingIdx].PCmdList;

    if (!PRootSignature)
    {
        return;
    }

    ID3D12RootSignature* Signature = static_cast<D3D12RootSignature*>(PRootSignature)->GetNative();
    switch (PipelineType)
    {
        case PipelineStateType_GRAPHICS:
        {
            // Set Graphics Root resources.
            
            CommandList->SetGraphicsRootSignature(Signature);
            break;
        }
        case PipelineStateType_COMPUTE:
        {
            CommandList->SetComputeRootSignature(Signature);
            break;
        }
    }
}


void D3D12GraphicsCommandList::DrawIndexedInstanced(U32 IndexCountPerInst,
                                                    U32 InstanceCount,
                                                    U32 StartIndexLocation,
                                                    I32 BaseVertexLocation,
                                                    U32 StartInstanceLocation)
{
    ID3D12GraphicsCommandList* CommandList = m_CommandLists[m_CurrentRecordingIdx].PCmdList;
    CommandList->DrawIndexedInstanced(IndexCountPerInst, InstanceCount, StartIndexLocation, 
        BaseVertexLocation, StartInstanceLocation);
}


void D3D12GraphicsCommandList::DrawInstanced(U32 VertexCountPerInstance,
                                             U32 InstanceCount,
                                             U32 StartVertexLocation,
                                             U32 StartInstanceLocation)
{
    ID3D12GraphicsCommandList* CommandList = m_CommandLists[m_CurrentRecordingIdx].PCmdList;
    CommandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}


void D3D12GraphicsCommandList::Dispatch(U32 X, U32 Y, U32 Z)
{
    ID3D12GraphicsCommandList* CommandList = m_CommandLists[m_CurrentRecordingIdx].PCmdList;
    CommandList->Dispatch(X, Y, Z);
}
} // Synthe