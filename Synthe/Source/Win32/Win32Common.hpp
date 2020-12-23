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

#ifdef _WIN32
#ifdef _WIN64
 #define ADDRESS_SIZE_BIT 64
 #define ADDRESS_SZ_MAX 0xFFFFFFFFFFFFFFFF
#else
 #define ADDRESS_SIZE_BIT 32
 #define ADDRESS_SZ_MAX 0xFFFFFFFF
#endif
#endif

#define INTEL_VENDOR_ID 0x8086
#define NVIDIA_VENDOR_ID 0x10DE 
#define NVIDIA_SGS_VENDOR_ID 0x12D2
#define AMD_VENDOR_ID 0x1002
#define AMD_ATI_VENDOR_ID 0x1022
#define QUALCOMM_VENDOR_ID 0x5143
#define MICROSOFT_VENDOR_ID 0x1414


#define BASE_CPU_DESCRIPTOR_ALLOC D3D12_CPU_DESCRIPTOR_HANDLE{ADDRESS_SZ_MAX}
#define BASE_GPU_DESCRIPTOR_ALLOC D3D12_GPU_DESCRIPTOR_HANDLE{ADDRESS_SZ_MAX}

//! Helper function for shader module translation into native code.
#define TRANSLATE_SHADER_MODULE(Desc, Native, Shader, NativeShader) { \
    if (Desc.Shader) { \
        Native.NativeShader.BytecodeLength = Desc.Shader->SizeInBytes; \
        Native.NativeShader.pShaderBytecode = Desc.Shader->ByteCode; \
    } \
}

//! Get the native DXGI format from the application defined formats. This corresponds to
//! the native format of the directx runtime module, which DirectX uses; The common format
//! is mainly for agnostic reasons.
//!
//! \param Format The common format defined by the application.
//! \return The native DXGI format that corresponds to the common GFormat.
DXGI_FORMAT GetCommonFormatToDXGIFormat(Synthe::PixelFormat Format);

//! Get the bit width, or pixel depth, of the input format. This should be the 
//! total size of a pixel that is to be provided by the buffer/image using this
//! format.
//!
//! \param Format The input format that we would like to know the bit width of.
//! \return The size in bits for the input format. 0 is returned if bit width is nonexistant,
//!         namely if the format is unknown.
UINT GetBitsForPixelFormat(DXGI_FORMAT Format);

//! Get the native D3D12 Resource Dimension from the common application value. 
//!
//! \param Dimension Common application dimension.
//! \return The D3D12 resource dimension. This is the native resource dimension used by D3D12 runtime.
D3D12_RESOURCE_DIMENSION GetResourceDimension(Synthe::ResourceDimension Dimension);

//! Generate a unique gpu handle for the application.
//! \return Unique identifer handled by the GPU device.
Synthe::GPUHandle GenerateNewHandle();