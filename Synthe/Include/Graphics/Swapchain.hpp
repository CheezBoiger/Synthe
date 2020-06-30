// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsStructs.hpp"
#include "GraphicsDevice.hpp"

namespace Synthe {


//! Basic Swapchain interface. Any form of presentation, or need to obtain info, or
//! resources from the swapchain, must be done here.
class Swapchain {
public:
    Swapchain() : m_Config() { }
    virtual ~Swapchain() { }

    virtual void Resize(const SwapchainConfig& Config) = 0;

    SwapchainConfig GetConfig() const { return m_Config; } 

    virtual ResultCode Present() { return SResult_NOT_IMPLEMENTED; }

    //! Get the current backbuffer render target view.
    //! 
    //! \return The GPU handle that corresponds to the current back buffer Render Target View.
    virtual GPUHandle GetCurrentBackBufferRTV() { return GPU_NO_HANDLE; }

protected:
    SwapchainConfig m_Config;
};
} // Synthe