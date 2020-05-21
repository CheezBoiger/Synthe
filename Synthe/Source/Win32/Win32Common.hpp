// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#pragma once

#include "Graphics/GraphicsStructs.hpp"

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include <dxgi.h>
#include <dxgi1_6.h>
#include <d3d12.h>

// TODO: Check Microsoft compiler version properly!!
#if _MSC_FULL_VER > 150000
#define DIRECTML_COMPATIBLE 1
#include <DirectML.h>
#endif


static DXGI_FORMAT GetCommonFormatToDXGIFormat(Synthe::GFormat Format)
{
    switch (Format)
    {
        case Synthe::GFormat_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case Synthe::GFormat_R16G16B16A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        default: return DXGI_FORMAT_UNKNOWN;
    }
}