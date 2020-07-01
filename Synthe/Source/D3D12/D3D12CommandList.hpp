// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Win32Common.hpp"
#include "Common/Types.hpp"
#include "Graphics/GraphicsCommandList.hpp"

#include <vector>

namespace Synthe {


struct ResourceState;


enum CommandState
{
    CommandState_READY = 0,
    CommandState_STILL_RECORDING,
    CommandState_STILL_EXECUTING,
};

class D3D12GraphicsCommandList : public GraphicsCommandList
{
private:
    struct CommandListState 
    {
        ID3D12CommandAllocator* PAllocatorRef;
        ID3D12GraphicsCommandList5* PCmdList;
        CommandState State;
    };
public:
    D3D12GraphicsCommandList()
        : m_CommandLists(0)
        , m_CurrentRecordingIdx(0)
        , m_DeviceRef(nullptr) { }

    ResultCode Initialize(ID3D12Device* PDevice, 
                          U32 NumCommandListBuffers,
                          ID3D12CommandAllocator** CommandAllocators,
                          D3D12_COMMAND_LIST_TYPE Type);

    void Release();
    void Begin() override;
    void End() override;

    void SetRenderTargets(U32 NumRTVs, GPUHandle* RTVHandles, GPUHandle* DepthStencil) override;
    
    void SetViewports(U32 NumViewports) override;

    void ClearRenderTarget(GPUHandle RTV, 
                           ClearColorValue* ClearColor, 
                           U32 NumBounds, 
                           TargetBounds* Bounds) override;

    void TransitionResourceIfNeeded(U32 NumHandles, GPUHandle* Descriptors, D3D12_RESOURCE_STATES* NeededStates);

    ID3D12GraphicsCommandList* GetNative() { return m_CommandLists[m_CurrentRecordingIdx].PCmdList; }
    void SetCurrentIdx(U32 Idx) { m_CurrentRecordingIdx = Idx; }

private:
    std::vector<CommandListState>   m_CommandLists;
    U32                             m_CurrentRecordingIdx;
    ID3D12Device*                   m_DeviceRef;
};
} // Synthe 