// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12Fence.hpp"


namespace Synthe {


ResultCode D3D12Fence::Initialize(ID3D12Device* PDevice, D3D12_FENCE_FLAGS Flags)
{
    m_Value = 1ULL;
    HRESULT Result = PDevice->CreateFence(m_Value, Flags, __uuidof(ID3D12Fence), (void**)&m_Native);
    if (FAILED(Result))
    {
        return SResult_INITIALIZATION_FAILURE;
    }
    return SResult_OK;
}


ResultCode D3D12Fence::Release()
{
    if (!m_Native)
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    m_Native->Release();
    m_Native = nullptr;
    m_Value = 0ULL;
    return SResult_OK;
}
} // Synthe