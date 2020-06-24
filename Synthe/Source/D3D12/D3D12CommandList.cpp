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


void D3D12GraphicsCommandList::Reset()
{
    m_ShouldReset = true;
}


void D3D12GraphicsCommandList::CheckShouldReset(ID3D12CommandAllocator* CommandAllocator)
{  
    CommandListState& State = m_CommandLists[m_CurrentRecordingIdx];
    if (m_ShouldReset)
    {
        State.PCmdList->Reset(CommandAllocator, nullptr);
    }
    m_ShouldReset = false;
}
} // Synthe