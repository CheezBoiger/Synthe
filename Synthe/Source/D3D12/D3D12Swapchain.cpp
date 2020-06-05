// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12Swapchain.hpp"
#include "D3D12GPUManager.hpp"

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
        return GResult_CREATION_FAILURE;
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

    QueryFrames(PDevice, SwapchainConfig.Count);
    QueryBufferingResources(PDevice, SwapchainConfig.Buffering);
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

    ID3D12Device* PDevice = nullptr;
    m_NativeHandle->GetDevice(__uuidof(ID3D12Device), (void**)&PDevice);
    QueryFrames(PDevice, m_Config.Count);
    if (Config.Buffering != m_Config.Buffering)
    {
        QueryBufferingResources(PDevice, Config.Buffering);
    }

    m_Config = Config;
}


void D3D12Swapchain::CleanUp()
{
    if (!m_NativeHandle)
        return;
    CleanUpFrameResources();
    CleanUpBufferingResources();
    m_NativeHandle->Release();
    m_BackbufferQueue->Release();
    m_NativeHandle = nullptr;
    m_BackbufferQueue = nullptr;
}


U32 D3D12Swapchain::GetNextFrameIndex()
{
    U32 FrameIndex = static_cast<U32>(m_NativeHandle->GetCurrentBackBufferIndex());
    BufferIndex = FrameIndex % (U32)m_BufferingResources.size();
    return FrameIndex;   
}


void D3D12Swapchain::SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex)
{
    FrameResource& Frame = m_FrameResources[FrameIndex];
    BufferingResource& Buffer = m_BufferingResources[FrameIndex % m_BufferingResources.size()];
    m_BackbufferQueue->Wait(Buffer.PWaitFence, Buffer.FenceSignalValue);
    m_BackbufferQueue->ExecuteCommandLists(Count, PPCommandLists);
    U32 FenceNewValue = Buffer.FenceSignalValue++;
    m_BackbufferQueue->Signal(Buffer.PSignalFence, FenceNewValue);
}


void D3D12Swapchain::QueryFrames(ID3D12Device* PDevice, U32 ImageCount)
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
        // TODO: We need to check if descriptor pool is initialized!
        Frame.RtvHandle = D3D12GPUMemoryManager::DescriptorPoolRtv().CreateRtv(PDevice, 
                                                                               RtvInfo, 
                                                                               Frame.PSwapchainImage, 
                                                                               Frame.RtvHandle);
    }
}


void D3D12Swapchain::CleanUpFrameResources()
{
    for (U32 I = 0; I < m_FrameResources.size(); ++I)
    {
        m_FrameResources[I].RtvHandle;
        m_FrameResources[I].PSwapchainImage = nullptr;
    }
}


void D3D12Swapchain::CleanUpBufferingResources()
{
    for (U32 I = 0; I < m_BufferingResources.size(); ++I) 
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        Buffer.PWaitFence->Release();
        Buffer.PSignalFence->Release();
        CloseHandle(Buffer.FenceEventSignal);
        CloseHandle(Buffer.FenceEventWait);
        Buffer.DirectCommandList->Release();
        Buffer.PCommandAllocator->Release();
    }  
}


void D3D12Swapchain::QueryBufferingResources(ID3D12Device* PDevice, U32 BufferingCount)
{
    CleanUpBufferingResources();
    m_BufferingResources.resize(BufferingCount);
    for (U32 I = 0; I < m_BufferingResources.size(); ++I)
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        
        Buffer.FenceEventSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
        Buffer.FenceEventWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    
        HRESULT Result = 0; 
        PDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Buffer.PSignalFence);
        PDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Buffer.PWaitFence);
        PDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&Buffer.PCommandAllocator);
        
        if (Buffer.PCommandAllocator)
            PDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Buffer.PCommandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&Buffer.DirectCommandList);
    }
}
} // Synthe 