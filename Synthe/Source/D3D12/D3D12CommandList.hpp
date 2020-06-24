// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Win32Common.hpp"
#include "Common/Types.hpp"
#include "Graphics/GraphicsCommandList.hpp"

#include <vector>

namespace Synthe {


enum CommandResult
{
    CommandResult_STILL_RECORDING,
    CommandResult_STILL_EXECUTING,
};


class D3D12GraphicsCommandList : public GraphicsCommandList
{
private:
    struct CommandListState 
    {
        ID3D12GraphicsCommandList* PCmdList;
    };
public:
    D3D12GraphicsCommandList()
        : m_CommandLists(0)
        , m_ShouldReset(false)
        , m_CurrentRecordingIdx(0) { }

    ResultCode Initialize(ID3D12Device* PDevice, 
                          U32 NumCommandListBuffers,
                          ID3D12CommandAllocator** CommandAllocators,
                          D3D12_COMMAND_LIST_TYPE Type);

    void Release();
    void Reset() override;

    ID3D12GraphicsCommandList* GetNative(U32 Idx = 0) { return m_CommandLists[Idx].PCmdList; }

    void CheckShouldReset(ID3D12CommandAllocator* CommandAllocator);
    void SetCurrentIdx(U32 Idx) { m_CurrentRecordingIdx = Idx; }

private:
    std::vector<CommandListState> m_CommandLists;
    B32 m_ShouldReset;
    U32 m_CurrentRecordingIdx;
};
} // Synthe 