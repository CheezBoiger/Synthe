// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "GraphicsStructs.hpp"
#include "GraphicsDevice.hpp"

namespace Synthe {


//! Basic Swapchain implementation.
class Swapchain {
public:
    Swapchain() : m_Config() { }
    virtual ~Swapchain() { }

    virtual void Resize(const SwapchainConfig& Config) = 0;

    SwapchainConfig GetConfig() const { return m_Config; } 
protected:
    SwapchainConfig m_Config;
};
} // Synthe