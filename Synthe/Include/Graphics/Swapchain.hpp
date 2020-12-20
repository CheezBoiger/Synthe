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

    //! Resize the image swapchain resolution.
    virtual void Resize(const SwapchainConfig& Config) = 0;

    //! Get the configurations of the swapchain.
    //!
    //! \return The Swapchain configuration data.
    SwapchainConfig GetConfig() const { return m_Config; } 

    //! Present the framebuffer to the window.
    //!
    //! \return SResult_OK if the present suceeds and submits for display. False otherwise,
    //!         where the result code will determine the reason of failure.
    virtual ResultCode Present() { return SResult_NOT_IMPLEMENTED; }

    //! Get the current backbuffer render target view.
    //! 
    //! \return The GPU handle that corresponds to the current back buffer Render Target View.
    virtual GPUHandle GetCurrentBackBufferRTV() { return SYNTHE_GPU_NO_HANDLE; }

protected:
    SwapchainConfig m_Config;
};
} // Synthe