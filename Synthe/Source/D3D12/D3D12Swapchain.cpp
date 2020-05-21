// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12Swapchain.hpp"


namespace Synthe {


void InitSwapchainDesc(DXGI_SWAP_CHAIN_DESC1& SwapchainDesc, const SwapchainConfig& SwapchainConfig)
{
    SwapchainDesc.BufferCount = SwapchainConfig.Count;
    SwapchainDesc.SampleDesc.Count = 1;
    SwapchainDesc.SampleDesc.Quality = 0;
    SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapchainDesc.Format = GetCommonFormatToDXGIFormat((GFormat)SwapchainConfig.Format);
    SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapchainDesc.Width = SwapchainConfig.Width;
    SwapchainDesc.Height = SwapchainConfig.Height;
    SwapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    SwapchainDesc.Scaling = DXGI_SCALING_NONE;
    SwapchainDesc.Stereo = FALSE;
    SwapchainDesc.Flags = 0;
}


GResult D3D12Swapchain::CreateBackbufferQueue(ID3D12Device* PDevice)
{
    D3D12_COMMAND_QUEUE_DESC Desc = { };
    Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    Desc.NodeMask = 0;
    Desc.Priority = 0;
    Desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    HRESULT HR = PDevice->CreateCommandQueue(&Desc, __uuidof(ID3D12CommandQueue), (void**)&m_BackbufferQueue);
    if (FAILED(HR)) 
        return GResult_CREATION_FAILTURE;
    return GResult_OK;
}


void D3D12Swapchain::Initialize(const SwapchainConfig& SwapchainConfig,
                                ID3D12Device* PDevice,
                                IDXGIFactory2* PFactory)
{
    DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = { };
    InitSwapchainDesc(SwapchainDesc, SwapchainConfig);

    if (CreateBackbufferQueue(PDevice) != GResult_OK)
        return;

    IDXGISwapChain1* Swapchain = nullptr;
    HRESULT Result = PFactory->CreateSwapChainForHwnd(m_BackbufferQueue, 
                                                      (HWND)SwapchainConfig.NativeWinHandle,
                                                      &SwapchainDesc, 
                                                      nullptr, 
                                                      nullptr, 
                                                      &Swapchain);
                                                      
    if (FAILED(Result))
    {
        return;
    }

    Swapchain->QueryInterface<IDXGISwapChain3>(&m_NativeHandle);
    m_Config = SwapchainConfig;
}


void D3D12Swapchain::Resize(const SwapchainConfig& Config)
{
    if (!m_NativeHandle)
        return;
    
    HRESULT Result = m_NativeHandle->ResizeBuffers(Config.Count, Config.Width, Config.Height, 
                                                   GetCommonFormatToDXGIFormat((GFormat)Config.Format), 0);
    if (FAILED(Result))
    {
        return;
    }

    m_Config = Config;
}


void D3D12Swapchain::CleanUp()
{
    if (!m_NativeHandle)
        return;
    m_NativeHandle->Release();
    m_BackbufferQueue->Release();
    m_NativeHandle = nullptr;
    m_BackbufferQueue = nullptr;
}


U32 D3D12Swapchain::GetNextFrameIndex()
{
    U32 FrameIndex = static_cast<U32>(m_NativeHandle->GetCurrentBackBufferIndex());
    return FrameIndex;   
}
} // Synthe 