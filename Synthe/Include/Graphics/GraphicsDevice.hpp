// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Graphics/GraphicsStructs.hpp"


namespace Synthe {


class GraphicsDevice {
public:

    virtual GResult Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                               const SwapchainConfig& SwapchainConfig) { return GResult_FAILED; }
    virtual GResult CleanUp() { return GResult_FAILED; }
    virtual GResult RecreateSwapchain() { return GResult_FAILED; }

    virtual GResult CreateResource(GPUHandle* Handle) { return GResult_FAILED; }
    virtual GResult CreateShaderResourceView(GPUHandle* Handle) { return GResult_FAILED; }
    virtual GResult CreateConstantBufferView(GPUHandle* Handle) { return GResult_FAILED; }
    virtual GResult CreateRenderTargetView(GPUHandle* Handle) { return GResult_FAILED; }
    virtual GResult CreateUnorderedAccessView(GPUHandle* Handle) { return GResult_FAILED; }
};


GraphicsDevice* CreateDeviceD3D12();
} // Synthe