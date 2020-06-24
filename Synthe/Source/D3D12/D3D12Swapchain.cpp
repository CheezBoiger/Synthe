// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12Swapchain.hpp"
#include "D3D12DescriptorManager.hpp"
#include "D3D12GraphicsDevice.hpp"

namespace Synthe {


void InitSwapchainDesc(DXGI_SWAP_CHAIN_DESC1& SwapchainDesc, const SwapchainConfig& SwapchainConfig)
{
    SwapchainDesc.BufferCount = SwapchainConfig.NumFrames;
    SwapchainDesc.SampleDesc.Count = 1;
    SwapchainDesc.SampleDesc.Quality = 0;
    SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapchainDesc.Format = GetCommonFormatToDXGIFormat(SwapchainConfig.Format);
    SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapchainDesc.Width = SwapchainConfig.Width;
    SwapchainDesc.Height = SwapchainConfig.Height;
    SwapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    SwapchainDesc.Scaling = DXGI_SCALING_NONE;
    SwapchainDesc.Stereo = FALSE;
    SwapchainDesc.Flags = 0;
}


void D3D12Swapchain::Initialize(const SwapchainConfig& SwapchainConfig,
                                ID3D12CommandQueue* PBackbufferQueue,
                                IDXGIFactory2* PFactory)
{
    DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = { };
    InitSwapchainDesc(SwapchainDesc, SwapchainConfig);

    IDXGISwapChain1* Swapchain = nullptr;
    HRESULT Result = PFactory->CreateSwapChainForHwnd(PBackbufferQueue, 
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

    QueryFrames(SwapchainConfig.NumFrames);
    if (m_Config.Flags & SwapchainFlags_ALLOW_TEARING)
    {
        m_PresentFlags &= DXGI_PRESENT_ALLOW_TEARING;
    }
}


void D3D12Swapchain::Resize(const SwapchainConfig& Config)
{
    if (!m_NativeHandle)
        return;
    
    HRESULT Result = m_NativeHandle->ResizeBuffers(Config.NumFrames, Config.Width, Config.Height, 
                                                   GetCommonFormatToDXGIFormat(Config.Format), 0);
    if (FAILED(Result))
    {
        return;
    }

    ID3D12Device* PDevice = nullptr;
    m_NativeHandle->GetDevice(__uuidof(ID3D12Device), (void**)&PDevice);
    QueryFrames(m_Config.NumFrames);

    m_Config = Config;
}


void D3D12Swapchain::CleanUp()
{
    if (!m_NativeHandle)
        return;
    CleanUpFrameResources();
    m_NativeHandle->Release();
    m_NativeHandle = nullptr;
}


U32 D3D12Swapchain::GetNextFrameIndex()
{
    U32 FrameIndex = static_cast<U32>(m_NativeHandle->GetCurrentBackBufferIndex());
    return FrameIndex;   
}


void D3D12Swapchain::QueryFrames(U32 ImageCount)
{
    CleanUpFrameResources();
    m_FrameResources.resize(ImageCount);

    DXGI_SWAP_CHAIN_DESC1 SwapchainDesc = { };
    m_NativeHandle->GetDesc1(&SwapchainDesc);

    for (U32 I = 0; I < ImageCount; ++I)
    {
        FrameResource& Frame = m_FrameResources[I];
        HRESULT Result = m_NativeHandle->GetBuffer(I, 
                                                   __uuidof(ID3D12Resource), 
                                                   (void**)&Frame.PSwapchainImage);
        if (FAILED(Result)) 
        {
            // Failed.  
            continue;  
        }
        D3D12_RENDER_TARGET_VIEW_DESC RtvInfo = { };
        RtvInfo.Format = SwapchainDesc.Format;
        RtvInfo.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        RtvInfo.Texture2D.MipSlice = 0;
        RtvInfo.Texture2D.PlaneSlice = 0;
    }
}


void D3D12Swapchain::CleanUpFrameResources()
{
    for (U32 I = 0; I < m_FrameResources.size(); ++I)
    {
        m_FrameResources[I].PSwapchainImage = nullptr;
    }
}


void D3D12Swapchain::BuildRTVs(ID3D12Device* PDevice, DescriptorPool* PPool)
{
    for (U32 I = 0; I < m_FrameResources.size(); ++I)
    {
        FrameResource& Frame = m_FrameResources[I];
        D3D12_RENDER_TARGET_VIEW_DESC RtvDesc = { };
        RtvDesc.Format = GetCommonFormatToDXGIFormat(m_Config.Format);
        RtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        RtvDesc.Texture2D.MipSlice = 0;
        RtvDesc.Texture2D.PlaneSlice = 0;
        Frame.ImageRTV = PPool->CreateRtv(PDevice, RtvDesc, Frame.PSwapchainImage, Frame.ImageRTV);
    }
}


ResultCode D3D12Swapchain::Present()
{
    HRESULT Result = m_NativeHandle->Present(m_Config.VSync, m_PresentFlags);
    if (FAILED(Result))
    {
        return GResult_FAILED;
    }
    return SResult_OK;
}
} // Synthe 