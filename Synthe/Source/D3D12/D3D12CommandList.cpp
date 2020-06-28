// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12CommandList.hpp"
#include "D3D12GraphicsDevice.hpp"


namespace Synthe {


ResultCode D3D12GraphicsCommandList::Initialize(ID3D12Device* PDevice, 
                                                U32 NumCommandListBuffers,
                                                ID3D12CommandAllocator** CommandAllocators,
                                                D3D12_COMMAND_LIST_TYPE Type)
{
    Release();

    m_CommandLists.resize(NumCommandListBuffers);
    for (U32 Idx = 0; Idx < m_CommandLists.size(); ++Idx)
    {
        HRESULT Result = PDevice->CreateCommandList(0, Type, CommandAllocators[Idx], nullptr, 
                            __uuidof(ID3D12GraphicsCommandList), (void**)&m_CommandLists[Idx].PCmdList);
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
} // Synthe