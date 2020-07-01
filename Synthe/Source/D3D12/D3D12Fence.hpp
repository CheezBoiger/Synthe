// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Graphics/Fence.hpp"
#include "Win32Common.hpp"


namespace Synthe {


class D3D12Fence : public Fence
{
public:
    ResultCode Initialize(ID3D12Device* PDevice, D3D12_FENCE_FLAGS Flags);
    ResultCode Release();

    ID3D12Fence* GetNativeFence() { return m_Native; }
    U64          GetCurrentValue() const { return m_Value; }
    void         SetValue(U64 Value) { m_Value = Value; }

    GPUHandle   m_Id;
private:
    ID3D12Fence* m_Native;
    U64          m_Value;
};
} // Synthe