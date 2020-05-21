// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once


#include "Win32Common.hpp"
#include "D3D12Swapchain.hpp"
#include "Graphics/GraphicsDevice.hpp"


namespace Synthe {


class D3D12GraphicsDevice : public GraphicsDevice {
public:

    GResult Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                       const SwapchainConfig& SwaphchainConfig) override;
    GResult CleanUp() override;

protected:
    
    U64 m_DedicatedVideoMemoryBytes;
    U64 m_DedicatedSystemMemoryBytes;
    U64 m_SharedMemoryBytes;
    GPUVendor m_Vendor;

private:
    
    ID3D12Device* m_Device;
    IDXGIFactory2* m_PFactory;
    IDXGIAdapter1* m_Adapter;
#if DIRECTML_COMPATIBLE
    IDMLDevice* m_MLDevice;
#endif
    D3D12Swapchain m_Swapchain;
};
} // Synthe