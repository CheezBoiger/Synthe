// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#ifndef D3D12_RESOURCE_VIEW_HPP
#define D3D12_RESOURCE_VIEW_HPP

#include "Common/Types.hpp"
#include "Win32Common.hpp"

#include "Graphics/GraphicsStructs.hpp"

namespace Synthe {

//! Obtain the Shader Resource View description in Native D3D12 context.
//!
ResultCode GetSrvDescription(const ShaderResourceViewCreateInfo& SRV, D3D12_SHADER_RESOURCE_VIEW_DESC& Desc);

//! Obtain the Unordered Access View description in native D3D12 context.
//!
ResultCode GetUavDescription(const UnorderedAccessViewCreateInfo& UAV, D3D12_DEPTH_STENCIL_VIEW_DESC& Desc);

//! Obtain the Render Target View Description in native D3D12 context.
//!
ResultCode GetRtvDescription(const RenderTargetViewCreateInfo& RTV, D3D12_RENDER_TARGET_VIEW_DESC& Desc);

//! Obtain the Depth Stencil View description in native D3D12 context.
//!
ResultCode GetDsvDescription(const DepthStencilViewCreateInfo& DSV, D3D12_DEPTH_STENCIL_VIEW_DESC& Desc);

} // Synthe 
#endif // D3D12_RESOURCE_VIEW_HPP