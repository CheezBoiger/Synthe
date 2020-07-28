// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GraphicsDevice.hpp"
#include "D3D12DescriptorManager.hpp"
#include "D3D12Resource.hpp"
#include "D3D12MemoryManager.hpp"

#include "D3D12Fence.hpp"

#include <array>

namespace Synthe {


GraphicsDevice* GetDeviceD3D12()
{
    // TODO: Figure out a better design to allocate our device.
    static D3D12GraphicsDevice* Device = nullptr;
    if (!Device)
    {
        Device = Malloc<D3D12GraphicsDevice>();
    }
    return Device;
}


void InitializeMemoryHeaps(ID3D12Device* PDevice)
{
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_SCENE, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_BUFFER, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_TEXTURE, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_SCRATCH, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_UPLOAD, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_READBACK, D3D12MemoryManager::AllocType_LINEAR);
    D3D12MemoryManager::CreateAndRegisterAllocator(MemoryType_RENDER_TARGETS_AND_DEPTH, D3D12MemoryManager::AllocType_LINEAR);
    
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_BUFFER);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_UPLOAD);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_SCRATCH);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_SCENE);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_TEXTURE);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_RENDER_TARGETS_AND_DEPTH);
    D3D12MemoryManager::CreateAndRegisterMemoryPool(MemoryType_READBACK);

    D3D12_HEAP_DESC HeapDesc = { };
    HeapDesc.Alignment = 0;
    HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
    HeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapDesc.Properties.CreationNodeMask = 0;
    HeapDesc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    HeapDesc.Properties.VisibleNodeMask = 0;
    U64 TotalSizeInBytes = MEM_1KB * MEM_BYTES(1024);
    HeapDesc.SizeInBytes = TotalSizeInBytes; 

    D3D12MemoryManager::GetAllocator(MemoryType_SCRATCH)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_SCRATCH)->Create(PDevice, 
        D3D12MemoryManager::GetAllocator(MemoryType_SCRATCH), HeapDesc);

    HeapDesc.SizeInBytes = MEM_1GB * MEM_BYTES(1);
    D3D12MemoryManager::GetAllocator(MemoryType_BUFFER)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_BUFFER)->Create(PDevice,
        D3D12MemoryManager::GetAllocator(MemoryType_BUFFER), HeapDesc);

    HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;
    HeapDesc.SizeInBytes = MEM_1MB * MEM_BYTES(512);
    D3D12MemoryManager::GetAllocator(MemoryType_RENDER_TARGETS_AND_DEPTH)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_RENDER_TARGETS_AND_DEPTH)->Create(PDevice,
        D3D12MemoryManager::GetAllocator(MemoryType_RENDER_TARGETS_AND_DEPTH), HeapDesc);

    HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
    HeapDesc.SizeInBytes = MEM_1MB * MEM_BYTES(64);
    HeapDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12MemoryManager::GetAllocator(MemoryType_UPLOAD)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_UPLOAD)->Create(PDevice,
        D3D12MemoryManager::GetAllocator(MemoryType_UPLOAD), HeapDesc);
    
    HeapDesc.Properties.Type = D3D12_HEAP_TYPE_READBACK;
    D3D12MemoryManager::GetAllocator(MemoryType_READBACK)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_READBACK)->Create(PDevice,
        D3D12MemoryManager::GetAllocator(MemoryType_READBACK), HeapDesc);

    HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES;
    HeapDesc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    HeapDesc.SizeInBytes = MEM_1MB * MEM_BYTES(512);
    HeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    D3D12MemoryManager::GetAllocator(MemoryType_TEXTURE)->Initialize(0ULL, HeapDesc.SizeInBytes);
    D3D12MemoryManager::GetMemoryPool(MemoryType_TEXTURE)->Create(PDevice,
        D3D12MemoryManager::GetAllocator(MemoryType_TEXTURE), HeapDesc);
}

void InitializeDescriptorHeaps(ID3D12Device* PDevice, U32 BufferingCount)
{
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_CBV_SRV_UAV, BufferingCount);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_CBV_SRV_UAV_UPLOAD, 1);
    
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_SAMPLER, BufferingCount);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_SAMPLER_UPLOAD, 1);

    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_RTV, 1U);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorHeapType_DSV, 1U);
    
    // Host Descriptor heaps.
    D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_RTV)->Create(PDevice, 
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 512);
    D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_DSV)->Create(PDevice,
        D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 2048);
    D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV_UPLOAD)->Create(
        PDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 8192);
    D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER_UPLOAD)->Create(
        PDevice, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1024);
    // Device Descriptor heaps.
    for (U32 I = 0; I < BufferingCount; ++I)
    {
        D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV, I)->Create(PDevice, 
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 8192);
        D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER, I)->Create(PDevice,
            D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1024);
    }
}


struct DeviceFeatureSupport
{
    D3D12_RAYTRACING_TIER RayTracing;
    D3D12_VARIABLE_SHADING_RATE_TIER VariableShadingRate;
    D3D12_CONSERVATIVE_RASTERIZATION_TIER ConservativeRaster;
    D3D12_TILED_RESOURCES_TIER TiledResources;
    D3D12_RESOURCE_HEAP_TIER ResourceHeapTier;
    BOOL BarycentricsSupported;
};


struct DeviceInfo
{
    GPUVendor Vendor;
    DeviceFeatureSupport FeatureSupport;
    U64 DedicatedVideoMemoryBytes;
    U64 DedicatedSystemMemoryBytes;
    U64 SharedMemoryBytes;
};


#define CHECK_FEATURE_SUPPORT(DeviceConfig, DesiredFlag, OldFlag, NewFlag, D3D12FeatureSupport) { \
    if (DeviceConfig.DesiredFlags & DesiredFlag) \
    { \
        if (NewFlag == D3D12FeatureSupport##_NOT_SUPPORTED) \
            return false; \
        if (NewFlag < OldFlag) \
            return false; \
    } \
}

BOOL CompareShouldReplace(const GraphicsDeviceConfig& DeviceConfig, const DeviceInfo& Old, const DeviceInfo& New)
{
    BOOL UseNewDevice = true;
    if (!(DeviceConfig.DesiredVendor & New.Vendor))
        return false;

    if (DeviceConfig.DesiresRequired)
    {
        CHECK_FEATURE_SUPPORT(DeviceConfig, GraphicsDeviceFlags_RAYTRACING_HARDWARE_BIT, 
                              Old.FeatureSupport.RayTracing, 
                              New.FeatureSupport.RayTracing, 
                              D3D12_RAYTRACING_TIER);
        CHECK_FEATURE_SUPPORT(DeviceConfig, GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT, 
                              Old.FeatureSupport.ConservativeRaster, 
                              New.FeatureSupport.ConservativeRaster, 
                              D3D12_CONSERVATIVE_RASTERIZATION_TIER);
        CHECK_FEATURE_SUPPORT(DeviceConfig, GraphicsDeviceFlags_VARIABLE_RATE_SHADING_BIT,
                              Old.FeatureSupport.VariableShadingRate,
                              New.FeatureSupport.VariableShadingRate,
                              D3D12_VARIABLE_SHADING_RATE_TIER);
        CHECK_FEATURE_SUPPORT(DeviceConfig, GraphicsDeviceFlags_TILED_RESOURCES_BIT,
                              Old.FeatureSupport.TiledResources,
                              New.FeatureSupport.TiledResources,
                              D3D12_TILED_RESOURCES_TIER);
        if (!(DeviceConfig.DesiredFlags & GraphicsDeviceFlags_BARYCENTRIC_COORDINATES_BIT))
            return false;
    } else
    {
        // Simple comparison of who has better.
        
    }
    return UseNewDevice;
}


void EnableDebugLayer()
{
    ID3D12Debug* Debug0 = nullptr;
    ID3D12Debug1* Debug1 = nullptr;
    D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&Debug0);
    Debug0->QueryInterface<ID3D12Debug1>(&Debug1);
    Debug0->EnableDebugLayer();
    Debug1->SetEnableGPUBasedValidation(true);
    Debug0->Release();
}


ResultCode D3D12GraphicsDevice::Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                                        const SwapchainConfig& SwapchainConfig)
{
    IDXGIFactory2* PFactory = nullptr;
    HRESULT Result = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&PFactory);
    if (FAILED(Result)) 
    {
        return GResult_INITIALIZATION_FAILURE;    
    }

    if (DeviceConfig.EnableDeviceDebugLayer)
    {
        EnableDebugLayer();
    }

    IDXGIAdapter1* BestAdapter = nullptr;
    ID3D12Device* BestDevice = nullptr;
    IDXGIAdapter1* TempAdapter = nullptr;
    DeviceInfo BestInfo = { };
    for (UINT i = 0; PFactory->EnumAdapters1(i, &TempAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC1 AdapterDesc = { };
        HRESULT AdapterResult = TempAdapter->GetDesc1(&AdapterDesc);
        DeviceInfo TempInfo = { };

        if (FAILED(AdapterResult))
        {
            continue;
        }

        // Check our vendor.
        {
            GPUVendor Vendor = GPUVendor_UNKNOWN;
            switch (AdapterDesc.VendorId)
            {
                case INTEL_VENDOR_ID: Vendor = GPUVendor_INTEL; break;
                case NVIDIA_VENDOR_ID:
                case NVIDIA_SGS_VENDOR_ID: Vendor = GPUVendor_NVIDIA; break;
                case AMD_ATI_VENDOR_ID: 
                case AMD_VENDOR_ID: Vendor = GPUVendor_AMD; break;
                case QUALCOMM_VENDOR_ID: Vendor = GPUVendor_QUALCOMM; break;
                case MICROSOFT_VENDOR_ID: Vendor = GPUVendor_MICROSOFT; break;
                default: Vendor = GPUVendor_UNKNOWN; break;
            }
        
            TempInfo.Vendor = Vendor;  
        }    

        TempInfo.SharedMemoryBytes = AdapterDesc.SharedSystemMemory;
        TempInfo.DedicatedVideoMemoryBytes = AdapterDesc.DedicatedVideoMemory;
        TempInfo.DedicatedSystemMemoryBytes = AdapterDesc.DedicatedSystemMemory;

        ID3D12Device* PDevice = nullptr;
        
        // Feature Level 11.1 is the minimum for DirectX.
        AdapterResult = D3D12CreateDevice(TempAdapter, D3D_FEATURE_LEVEL_11_1, __uuidof(ID3D12Device), (void**)&PDevice);
        if (FAILED(AdapterResult))
            continue;
        // Check for feature support.
        {
            D3D12_FEATURE_DATA_D3D12_OPTIONS FeatureOptions = { };
            TempInfo.FeatureSupport.ConservativeRaster = FeatureOptions.ConservativeRasterizationTier;
            TempInfo.FeatureSupport.TiledResources = FeatureOptions.TiledResourcesTier;
            TempInfo.FeatureSupport.ResourceHeapTier = FeatureOptions.ResourceHeapTier;
        }

        {
            D3D12_FEATURE_DATA_D3D12_OPTIONS3 FeatureOptions = { };
            TempInfo.FeatureSupport.BarycentricsSupported = FeatureOptions.BarycentricsSupported;
        }

        {
            D3D12_FEATURE_DATA_D3D12_OPTIONS5 FeatureOptions = { };
            PDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &FeatureOptions, sizeof(FeatureOptions));
            // Ray tracing check.
            TempInfo.FeatureSupport.RayTracing = FeatureOptions.RaytracingTier;
        }

        {
            D3D12_FEATURE_DATA_D3D12_OPTIONS6 FeatureOptions6 = { };
            PDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &FeatureOptions6, sizeof(FeatureOptions6));
            TempInfo.FeatureSupport.VariableShadingRate = FeatureOptions6.VariableShadingRateTier;
        }

        
        if (CompareShouldReplace(DeviceConfig, BestInfo, TempInfo)) {
            if (BestDevice) BestDevice->Release();
            BestInfo = TempInfo;
            BestDevice = PDevice;
            BestAdapter = TempAdapter;
        }
    }

    if (!BestAdapter)
    {
        return GResult_INITIALIZATION_FAILURE;
    }

    m_Device = BestDevice;
    m_Adapter = BestAdapter;
    BestDevice->QueryInterface<ID3D12Device5>(&m_AdvDevice);

    m_Features.Vendor = BestInfo.Vendor;
    m_Features.SharedSystemMemory = BestInfo.SharedMemoryBytes;
    m_Features.DedicatedSystemMemoryInBytes = BestInfo.DedicatedSystemMemoryBytes;
    m_Features.DedicatedVideoMemoryInBytes = BestInfo.DedicatedVideoMemoryBytes;
    m_ResourceHeapTier = BestInfo.FeatureSupport.ResourceHeapTier;

    InitializeMemoryHeaps(m_Device);
    InitializeDescriptorHeaps(m_Device, SwapchainConfig.Buffering);    
    CreateGraphicsQueue();
    CreateAsyncQueue();
    CreateCopyQueue();
    // Initialize our swapchain.
    m_Swapchain.Initialize(SwapchainConfig, m_GraphicsQueue, PFactory);
    // Initialize RTVs for swapchain.
    m_Swapchain.BuildRTVs(m_Device, D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_RTV));

    QueryBufferingResources(SwapchainConfig.Buffering);

    m_PFactory = PFactory;

    // Wait on GPU.
    WaitOnGPU();

    return SResult_OK;
}


void D3D12GraphicsDevice::SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex)
{
    BufferingResource& Buffer = m_BufferingResources[FrameIndex % m_BufferingResources.size()];
    m_GraphicsQueue->ExecuteCommandLists(Count, PPCommandLists);
}


ResultCode D3D12GraphicsDevice::CleanUp()
{
    m_Swapchain.CleanUp();
    CleanUpFences();
    if (m_GraphicsQueue)
        m_GraphicsQueue->Release();
    if (m_AsyncQueue)
        m_AsyncQueue->Release();
    if (m_CopyQueue)
        m_CopyQueue->Release();
    if (m_PFactory)
        m_PFactory->Release();
    if (m_Device)
        m_Device->Release();
    if (m_MLDevice)
        m_MLDevice->Release();
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateGraphicsQueue()
{
    D3D12_COMMAND_QUEUE_DESC Desc = { };
    Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    Desc.NodeMask = 0;
    Desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
    Desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    HRESULT HR = m_Device->CreateCommandQueue(&Desc, __uuidof(ID3D12CommandQueue), (void**)&m_GraphicsQueue);
    if (FAILED(HR)) 
        return GResult_DEVICE_CREATION_FAILURE;
    return SResult_OK;
}


void D3D12GraphicsDevice::CleanUpBufferingResources()
{
    for (U32 I = 0; I < m_BufferingResources.size(); ++I) 
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        Buffer.PWaitFence->Release();
        CloseHandle(Buffer.FenceEventWait);
        Buffer.PCommandAllocator->Release();
    }  
    m_BackbufferCommandList.Release();
}


void D3D12GraphicsDevice::QueryBufferingResources(U32 BufferingCount)
{
    CleanUpBufferingResources();
    m_BufferingResources.resize(BufferingCount);
    std::vector<ID3D12CommandAllocator*> Allocators(BufferingCount);
    for (U32 I = 0; I < m_BufferingResources.size(); ++I)
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        
        Buffer.FenceEventWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        HRESULT Result = 0; 
        m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Buffer.PWaitFence);
        m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&Buffer.PCommandAllocator);
        Buffer.FenceWaitValue = 1ULL;
        Allocators[I] = Buffer.PCommandAllocator;
    }
    m_BackbufferCommandList.Initialize(m_Device, BufferingCount, Allocators.data(), D3D12_COMMAND_LIST_TYPE_DIRECT);
}


U64 D3D12GraphicsDevice::GetCurrentUsedMemoryBytesInPool(U64 Key)
{
    Allocator* PAllocator = D3D12MemoryManager::GetAllocator(Key);
    if (!PAllocator)
    {
        return 0ULL;
    }
    return PAllocator->GetCurrentUsedBytes();
}


U64 D3D12GraphicsDevice::GetTotalSizeMemoryBytesForPool(U64 Key)
{
    Allocator* PAllocator = D3D12MemoryManager::GetAllocator(Key);
    if (!PAllocator)
    {
        return 0ULL;
    }
    return PAllocator->GetTotalSizeBytes();
}


ResultCode D3D12GraphicsDevice::Present()
{
    const FrameResource& Frame = m_Swapchain.GetFrameResource(m_Swapchain.GetCurrentFrameIndex());
    ResourceState ResourceS;
    D3D12MemoryManager::GetNativeResource(Frame.ResourceHandle, &ResourceS);
    if (ResourceS.State != D3D12_RESOURCE_STATE_PRESENT)
    {
        m_BackbufferCommandList.Begin();
            D3D12_RESOURCE_STATES State = D3D12_RESOURCE_STATE_PRESENT;
            m_BackbufferCommandList.TransitionResourceIfNeeded(1, (GPUHandle*)&Frame.ImageRTV.ptr, &State);
        m_BackbufferCommandList.End();
        D3D12MemoryManager::UpdateResourceState(Frame.ResourceHandle, D3D12_RESOURCE_STATE_PRESENT);
        ID3D12CommandList* CmdList[] = { m_BackbufferCommandList.GetNative() };
        m_GraphicsQueue->ExecuteCommandLists(1, CmdList);
    }
    return m_Swapchain.Present();
}


void D3D12GraphicsDevice::Begin()
{
    BufferingResource& Buffer = m_BufferingResources[m_BufferIndex];
    Buffer.PCommandAllocator->Reset();

    // Update our per frame command lists.
    for (auto* PCommandList : m_PerFrameCommandLists)
    {
        PCommandList->SetCurrentIdx(m_BufferIndex);
    }
    // Update our backbuffer commandlist too.
    m_BackbufferCommandList.SetCurrentIdx(m_BufferIndex);

    D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV, m_BufferIndex)->ResetPool();
    for (std::pair<const GPUHandle, D3D12DescriptorSet*>& Set : m_DescriptorSets)
    {
        Set.second->UploadToShaderVisibleHeap(m_Device, m_BufferIndex);
    }
}


void D3D12GraphicsDevice::End()
{
    BufferingResource& Buffer = m_BufferingResources[m_BufferIndex];
    m_GraphicsQueue->Signal(Buffer.PWaitFence, Buffer.FenceWaitValue);
    // Next frame to work on.
    m_BufferIndex = m_Swapchain.GetCurrentFrameIndex() % static_cast<U32>(m_BufferingResources.size());
    if (Buffer.PWaitFence->GetCompletedValue() < Buffer.FenceWaitValue)
    {
        Buffer.PWaitFence->SetEventOnCompletion(Buffer.FenceWaitValue, Buffer.FenceEventWait);
        WaitForSingleObject(Buffer.FenceEventWait, INFINITE);
    }
    Buffer.FenceWaitValue += 1;
}


void D3D12GraphicsDevice::WaitOnGPU()
{
    BufferingResource& Buffer = m_BufferingResources[m_BufferIndex];
    m_GraphicsQueue->Signal(Buffer.PWaitFence, Buffer.FenceWaitValue);
    Buffer.PWaitFence->SetEventOnCompletion(Buffer.FenceWaitValue, Buffer.FenceEventWait);
    WaitForSingleObject(Buffer.FenceEventWait, INFINITE);
    Buffer.FenceWaitValue += 1;
}


ResultCode D3D12GraphicsDevice::CreateResource(GPUHandle* Out, 
                                               const ResourceCreateInfo* PCreateInfo, 
                                               const ClearValue* PClearValue)
{
    *Out = GenerateNewHandle();
    ID3D12Resource* PResource = nullptr;
    
    MemoryType MemType = MemoryType_BUFFER;
    switch (PCreateInfo->Dimension)
    {
        case ResourceDimension_BUFFER:
            MemType = MemoryType_BUFFER;
            break;
        default:
            MemType = MemoryType_TEXTURE;
            break;
    }
    
    D3D12_RESOURCE_DESC ResourceDesc = { };
    ResourceDesc.Width = PCreateInfo->Width;
    ResourceDesc.Height = PCreateInfo->Height;
    ResourceDesc.DepthOrArraySize = PCreateInfo->DepthOrArraySize;
    ResourceDesc.Format = GetCommonFormatToDXGIFormat(PCreateInfo->ResourceFormat);
    ResourceDesc.Alignment = 0ULL;
    ResourceDesc.Dimension = GetResourceDimension(PCreateInfo->Dimension);
    ResourceDesc.MipLevels = PCreateInfo->Mips;
    ResourceDesc.SampleDesc.Count = static_cast<U32>(PCreateInfo->SampleCount);
    ResourceDesc.SampleDesc.Quality  = static_cast<U32>(PCreateInfo->SampleQuality);
    
    if (ResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    {    
        ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    }
    else
    {
        ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    }

    D3D12_CLEAR_VALUE ClearValue = { };

    if (PClearValue)
    {
        ClearValue.Color[0] = PClearValue->Color[0];
        ClearValue.Color[1] = PClearValue->Color[1];
        ClearValue.Color[2] = PClearValue->Color[2];
        ClearValue.Color[3] = PClearValue->Color[3];
        ClearValue.DepthStencil.Depth = PClearValue->Depth;
        ClearValue.DepthStencil.Stencil = PClearValue->Stencil;
        ClearValue.Format = ResourceDesc.Format;
    }

    D3D12_RESOURCE_STATES InitialState = D3D12_RESOURCE_STATE_COMMON;

    ResultCode Result = D3D12MemoryManager::GetMemoryPool(MemType)->AllocateResource(m_Device, 
        ResourceDesc, InitialState, 
        PClearValue ? &ClearValue : nullptr, 
        &PResource);

    if (Result == SResult_OK) 
    {
        ResultCode CacheResult = D3D12MemoryManager::CacheNativeResource(*Out, PResource, InitialState);
    } 
    else 
    {
        // Handle error.
    }

    return Result;
}


ResultCode D3D12GraphicsDevice::CreateCommandList(CommandListCreateInfo& Info, GraphicsCommandList** PList)
{
    ResultCode Code = SResult_OK;
    D3D12_COMMAND_LIST_TYPE CommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
    switch (Info.Type)
    {
        case CommandListType_COPY:
            CommandListType = D3D12_COMMAND_LIST_TYPE_COPY;
            break;
        case CommandListType_COMPUTE:
            CommandListType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            break;
        case CommandListType_GRAPHICS:
        default:
            CommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
            break;
    }

    if (Info.Level == CommandListLevel_SECONDARY)
    {
        CommandListType = D3D12_COMMAND_LIST_TYPE_BUNDLE;
    }

    if (Info.Flags & CommandListFlag_RARELY_UPDATE) 
    {
    }
    else
    {
        D3D12GraphicsCommandList* D3DCommandList = Malloc<D3D12GraphicsCommandList>(D3D12GraphicsCommandList());
        // We need to update for each command list.
        std::vector<ID3D12CommandAllocator*> Allocators(m_BufferingResources.size());
        for (U32 I = 0; I < m_BufferingResources.size(); ++I)
        {
            Allocators[I] = m_BufferingResources[I].PCommandAllocator;
        }
        U32 NumBuffers = static_cast<U32>(m_BufferingResources.size());
        Code = D3DCommandList->Initialize(m_Device, NumBuffers, Allocators.data(), CommandListType);
        if (Code == SResult_OK)
        {
            m_PerFrameCommandLists.push_back(D3DCommandList);
            *PList = m_PerFrameCommandLists.back();
        } 
        else 
        {
            Free<D3D12GraphicsCommandList>(D3DCommandList);
        }
    }
    
    return Code;
}


ResultCode D3D12GraphicsDevice::SubmitCommandLists(U32 NumSubmits,
                                                   const CommandListSubmitInfo* PSubmitInfos)
{
    static ID3D12CommandList* CmdListBuffer[32];
    ResultCode Code = SResult_OK;

    for (U32 I = 0; I < NumSubmits; ++I)
    {
        ID3D12CommandQueue* Queue = nullptr;
        const CommandListSubmitInfo& Info = PSubmitInfos[I];
        switch (Info.QueueToSubmit)
        {
            case SubmitQueue_ASYNC:
                Queue = m_AsyncQueue;
                break;
            case SubmitQueue_COPY:
                Queue = m_CopyQueue;
                break;
            case SubmitQueue_GRAPHICS:
            default:
                Queue = m_GraphicsQueue;
                break;
        } 
        for (U32 CmdIdx = 0; CmdIdx < Info.NumCommandLists; ++CmdIdx)
        {
            CmdListBuffer[CmdIdx] = static_cast<D3D12GraphicsCommandList*>(
                Info.PCmdLists[CmdIdx])->GetNative();
        }
        for (U32 I = 0; I < Info.NumWaitFences; ++I)
        {
            D3D12Fence* PFence = static_cast<D3D12Fence*>(Info.WaitFences[I]);
            Queue->Wait(PFence->GetNativeFence(), PFence->GetCurrentValue());   
        }
        Queue->ExecuteCommandLists(Info.NumCommandLists, CmdListBuffer);
        for (U32 I = 0; I < Info.NumSignalFences; ++I)
        {
            D3D12Fence* PFence = static_cast<D3D12Fence*>(Info.SignalFences[I]);
            PFence->SetValue(PFence->GetCurrentValue() + 1ULL);
            Queue->Signal(PFence->GetNativeFence(), PFence->GetCurrentValue());
        }
    }

    return Code;
}


ResultCode D3D12GraphicsDevice::CreateAsyncQueue()
{
    D3D12_COMMAND_QUEUE_DESC Desc = { };
    Desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
    Desc.NodeMask = 0;
    Desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    HRESULT Result = m_Device->CreateCommandQueue(&Desc, __uuidof(ID3D12CommandQueue), (void**)&m_AsyncQueue);
    if (FAILED(Result))
    {
        return SResult_FAILED;
    }
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateCopyQueue()
{
    D3D12_COMMAND_QUEUE_DESC Desc = { };
    Desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
    Desc.NodeMask = 0;
    Desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    HRESULT Result = m_Device->CreateCommandQueue(&Desc, __uuidof(ID3D12CommandQueue), (void**)&m_CopyQueue);
    if (FAILED(Result))
    {
        return SResult_FAILED;
    }
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::DestroyCommandLists(U32 NumCommandLists, GraphicsCommandList** CommandLists)
{
    for (U32 I = 0; I < NumCommandLists; ++I)
    {
        D3D12GraphicsCommandList* CmdListD3D = static_cast<D3D12GraphicsCommandList*>(CommandLists[I]);
        CmdListD3D->Release();
        // Find the command list in per frame, if the command list is dynamic.
    }
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateRenderTargetView(const RenderTargetViewCreateInfo& RTV,
                                                       GPUHandle* OutHandle)
{
    D3D12_RENDER_TARGET_VIEW_DESC Desc = { };
    Desc.Format = GetCommonFormatToDXGIFormat(RTV.Format);
    switch (RTV.Dimension)
    {
        case RTVViewDimension_BUFFER:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_BUFFER;
            Desc.Buffer.FirstElement = RTV.Buffer.FirstElement;
            Desc.Buffer.NumElements = RTV.Buffer.NumElements;
            break;
        case RTVViewDimension_TEXTURE_1D:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipSlice = RTV.Texture1D.MipSlice;
            break;
        case RTVViewDimension_TEXTURE_2D:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipSlice = RTV.Texture2D.MipSlice;
            Desc.Texture2D.PlaneSlice = RTV.Texture2D.PlaneSlice;
            break;
        case RTVViewDimension_TEXTURE_1D_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = RTV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = RTV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipSlice = RTV.Texture1DArray.MipSlice;
            break;
        case RTVViewDimension_TEXTURE_2D_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = RTV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = RTV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipSlice = RTV.Texture2DArray.MipSlice;
            Desc.Texture2DArray.PlaneSlice = RTV.Texture2DArray.PlaneSlice;
            break;
        case RTVViewDimension_TEXTURE_2DMS_ARRAY:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice;
            break;
        case RTVViewDimension_TEXTURE_2DMS:
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        case RTVViewDimension_TEXTURE_3D:
            Desc.Texture3D.FirstWSlice = RTV.Texture3D.FirstWSlice;
            Desc.Texture3D.MipSlice = RTV.Texture3D.MipSlice;
            Desc.Texture3D.WSize = RTV.Texture3D.WSize;
            break;
        default:
            // No proper dimension was set.
            return SResult_INVALID_ARGS;
    }
    ResourceState ResourceStateO = { };
    D3D12MemoryManager::GetNativeResource(RTV.ResourceHandle, &ResourceStateO);
    if (!ResourceStateO.PResource)
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    // Just return the pointer as a gpu handle.
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle = 
        D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_RTV)->CreateRtv(
            m_Device, Desc, ResourceStateO.PResource);
    *OutHandle = RtvHandle.ptr;
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateFence(Fence** OutFence)
{
    D3D12Fence* PFence = Malloc<D3D12Fence>();
    ResultCode Result = PFence->Initialize(m_Device, D3D12_FENCE_FLAG_NONE);
    if (Result == SResult_OK)
    {
        GPUHandle Handle = GenerateNewHandle(); 
        PFence->m_Id = Handle;
        m_Fences[Handle] = PFence;
        *OutFence = m_Fences[Handle];
    } 
    else 
    {
        Free<D3D12Fence>(PFence);
    }
    return Result;
}


void D3D12GraphicsDevice::CleanUpFences()
{
    for (auto& RFence : m_Fences)
    {
        RFence.second->Release();
        Free<D3D12Fence>(RFence.second);
    }
    m_Fences.clear();
}


void D3D12GraphicsDevice::ReleaseAsyncQueue()
{
    if (m_AsyncQueue)
    {
        m_AsyncQueue->Release();
    }
    m_AsyncQueue = nullptr;
}


void D3D12GraphicsDevice::ReleaseCopyQueue()
{
    if (m_CopyQueue)
    {
        m_CopyQueue->Release();
    }
    m_CopyQueue = nullptr;
}


ResultCode D3D12GraphicsDevice::CreateRootSignature(RootSignature** PRootSignature, 
                                                    const RootSignatureLayoutInfo& CreateInfo)
{
    ResultCode OutResult = SResult_OK;
    D3D12_ROOT_SIGNATURE_DESC RootSigDesc = { };
    
    std::vector<D3D12_ROOT_PARAMETER> DescriptorTableLayouts(CreateInfo.NumDescriptorTables);
    std::vector<std::array<D3D12_DESCRIPTOR_RANGE, 4>> Ranges(CreateInfo.NumDescriptorTables);
    
    for (U32 I = 0; I < DescriptorTableLayouts.size(); ++I)
    {
        DescriptorTableLayouts[I].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        D3D12_DESCRIPTOR_RANGE SrvRange;
        D3D12_DESCRIPTOR_RANGE CbvRange;
        D3D12_DESCRIPTOR_RANGE UavRange;
        D3D12_DESCRIPTOR_RANGE SamplerRange;    

        SrvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        CbvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        UavRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        SamplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

        U32 NumRanges = 0;
        if (CreateInfo.LayoutInfos[I].Srv.NumDescriptors) 
        {
            SrvRange.NumDescriptors = CreateInfo.LayoutInfos[I].Srv.NumDescriptors;
            SrvRange.BaseShaderRegister = CreateInfo.LayoutInfos[I].Srv.BaseRegister;
            SrvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            SrvRange.RegisterSpace = 0;
            Ranges[I][NumRanges++] = SrvRange;
        }
        if (CreateInfo.LayoutInfos[I].Cbv.NumDescriptors)
        {
            CbvRange.NumDescriptors = CreateInfo.LayoutInfos[I].Cbv.NumDescriptors;
            CbvRange.BaseShaderRegister = CreateInfo.LayoutInfos[I].Cbv.BaseRegister;
            CbvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            CbvRange.RegisterSpace = 0;
            Ranges[I][NumRanges++] = CbvRange;
        }
        if (CreateInfo.LayoutInfos[I].Uav.NumDescriptors)
        {
            UavRange.NumDescriptors = CreateInfo.LayoutInfos[I].Uav.NumDescriptors;
            UavRange.BaseShaderRegister = CreateInfo.LayoutInfos[I].Uav.BaseRegister;
            UavRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            UavRange.RegisterSpace = 0;
            Ranges[I][NumRanges++] = UavRange;
        }
        DescriptorTableLayouts[I].DescriptorTable.pDescriptorRanges = Ranges[I].data();
        DescriptorTableLayouts[I].DescriptorTable.NumDescriptorRanges = NumRanges;
    }
    

    if (DescriptorTableLayouts.size()) 
    {
        RootSigDesc.NumParameters = CreateInfo.NumDescriptorTables;
        RootSigDesc.pParameters = DescriptorTableLayouts.data();
        ID3DBlob* PBlob = nullptr;
        ID3DBlob* PErrorBlob = nullptr;
        ID3D12RootSignature* PNativeRootSignature = nullptr;
        
        HRESULT Result = D3D12SerializeRootSignature(&RootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, 
            &PBlob, &PErrorBlob);
        
        if (FAILED(Result)) 
        {
            OutResult = GResult_ROOT_SIGNATURE_SERIALIZATION_ERROR;
        } 
        else
        {
            Result = m_Device->CreateRootSignature(0, PBlob->GetBufferPointer(), PBlob->GetBufferSize(), 
                __uuidof(ID3D12RootSignature), (void**)&PNativeRootSignature);
            if (FAILED(Result))
            {
                OutResult = GResult_ROOT_SIGNATURE_CREATION_ERROR;
            } 
            else 
            {
                *PRootSignature = Malloc<D3D12RootSignature>(PNativeRootSignature);
            }
        }
        if (PBlob) PBlob->Release();
        if (PErrorBlob) PErrorBlob->Release();
    }
    return OutResult;
}


ResultCode D3D12GraphicsDevice::CreateShaderResourceView(const ShaderResourceViewCreateInfo& SRV,
                                                         GPUHandle* OutHandle)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC Desc = { };
    Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    Desc.Format = GetCommonFormatToDXGIFormat(SRV.Format);
    switch (SRV.Dimension)
    {
        case SrvViewDimension_BUFFER:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            Desc.Buffer.FirstElement = SRV.Buffer.FirstElement;
            Desc.Buffer.Flags = (D3D12_BUFFER_SRV_FLAGS)SRV.Buffer.Flags;
            Desc.Buffer.NumElements = SRV.Buffer.NumElements;
            Desc.Buffer.StructureByteStride = SRV.Buffer.StructureByteStride;
            break;
        }
        case SrvViewDimension_RAYTRACING_ACCELERATION_STRUCTURE:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
            Desc.RaytracingAccelerationStructure.Location = SRV.RaytracingAccelerationStructure.Location;
            break;
        }
        case SrvViewDimension_TEXTURE1D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipLevels = SRV.Texture1D.MipLevels;
            Desc.Texture1D.MostDetailedMip = SRV.Texture1D.MostDetailedMip;
            Desc.Texture1D.ResourceMinLODClamp = SRV.Texture1D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE1D_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = SRV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = SRV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipLevels = SRV.Texture1DArray.MipLevels;
            Desc.Texture1DArray.MostDetailedMip = SRV.Texture1DArray.MostDetailedMip;
            Desc.Texture1DArray.ResourceMinLODClamp = SRV.Texture1DArray.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE2D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipLevels = SRV.Texture2D.MipLevels;
            Desc.Texture2D.MostDetailedMip = SRV.Texture2D.MostDetailedMip;
            Desc.Texture2D.PlaneSlice = SRV.Texture2D.PlaneSlice;
            Desc.Texture2D.ResourceMinLODClamp = SRV.Texture2D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE2DMS:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        }
        case SrvViewDimension_TEXTURE2DMS_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize = SRV.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice = SRV.Texture2DMSArray.FirstArraySlice;
            break;
        }
        case SrvViewDimension_TEXTURE2D_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = SRV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = SRV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipLevels = SRV.Texture2DArray.MipLevels;
            Desc.Texture2DArray.MostDetailedMip = SRV.Texture2DArray.MostDetailedMip;
            Desc.Texture2DArray.PlaneSlice = SRV.Texture2DArray.PlaneSlice;
            Desc.Texture2DArray.ResourceMinLODClamp = SRV.Texture2DArray.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE3D:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
            Desc.Texture3D.MipLevels = SRV.Texture3D.MipLevels;
            Desc.Texture3D.MostDetailedMip = SRV.Texture3D.MostDetailedMip;
            Desc.Texture3D.ResourceMinLODClamp = SRV.Texture3D.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE_CUBE:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
            Desc.TextureCube.MipLevels = SRV.TextureCube.MipLevels;
            Desc.TextureCube.MostDetailedMip = SRV.TextureCube.MostDetailedMip;
            Desc.TextureCube.ResourceMinLODClamp = SRV.TextureCube.ResourceMinLODClamp;
            break;
        }
        case SrvViewDimension_TEXTURE_CUBE_ARRAY:
        {
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
            Desc.TextureCubeArray.First2DArrayFace = SRV.TextureCubeArray.First2DArrayFace;
            Desc.TextureCubeArray.MipLevels = SRV.TextureCubeArray.MipLevels;
            Desc.TextureCubeArray.MostDetailedMip = SRV.TextureCubeArray.MostDetailedMip;
            Desc.TextureCubeArray.NumCubes = SRV.TextureCubeArray.NumCubes;
            Desc.TextureCubeArray.ResourceMinLODClamp = SRV.TextureCubeArray.ResourceMinLODClamp;
            break;
        }
    }
    ResourceState ResourceStateO;
    D3D12MemoryManager::GetNativeResource(SRV.ResourceHandle, &ResourceStateO);
    if (!ResourceStateO.PResource)
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    DescriptorPool* Pool = D3D12DescriptorManager::GetDescriptorPool(
        DescriptorHeapType_CBV_SRV_UAV_UPLOAD);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = Pool->CreateSrv(m_Device, Desc, ResourceStateO.PResource);
    *OutHandle = Handle.ptr;
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateDepthStencilView(const DepthStencilViewCreateInfo& DSV,
                                                       GPUHandle* OutHandle)
{
    D3D12_DEPTH_STENCIL_VIEW_DESC Desc = { };
    Desc.Format = GetCommonFormatToDXGIFormat(DSV.Format);
    switch (DSV.Dimension)
    {
        case DSVViewDimension_TEXTURE1D:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
            Desc.Texture1D.MipSlice = DSV.Texture1D.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE1D_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
            Desc.Texture1DArray.ArraySize = DSV.Texture1DArray.ArraySize;
            Desc.Texture1DArray.FirstArraySlice = DSV.Texture1DArray.FirstArraySlice;
            Desc.Texture1DArray.MipSlice = DSV.Texture1DArray.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE2D:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipSlice = DSV.Texture2D.MipSlice;
            break;
        }
        case DSVViewDimension_TEXTURE2DMS:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
            Desc.Texture2DMS.UnusedField_NothingToDefine;
            break;
        }
        case DSVViewDimension_TEXTURE2DMS_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
            Desc.Texture2DMSArray.ArraySize = DSV.Texture2DMSArray.ArraySize;
            Desc.Texture2DMSArray.FirstArraySlice = DSV.Texture2DMSArray.FirstArraySlice;
            break;
        }
        case DSVViewDimension_TEXTURE2D_ARRAY:
        {
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
            Desc.Texture2DArray.ArraySize = DSV.Texture2DArray.ArraySize;
            Desc.Texture2DArray.FirstArraySlice = DSV.Texture2DArray.FirstArraySlice;
            Desc.Texture2DArray.MipSlice = DSV.Texture2DArray.MipSlice;
            break;
        }
    }
    ResourceState RSO;
    D3D12MemoryManager::GetNativeResource(DSV.ResourceHandle, &RSO);
    if (!RSO.PResource)
    {
        return SResult_OBJECT_NOT_FOUND;
    }
    DescriptorPool* Pool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV_UPLOAD);
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = Pool->CreateDsv(m_Device, Desc, RSO.PResource);
    *OutHandle = Handle.ptr;
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::AllocateDescriptorSets(U32 NumDescriptorSets, DescriptorSet** POutSets,
    DescriptorSetLayoutInfo* PLayouts)
{
    DescriptorPool* CBVSRVUAVPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_CBV_SRV_UAV);
    DescriptorPool* SamplerPool = D3D12DescriptorManager::GetDescriptorPool(DescriptorHeapType_SAMPLER);
    U64 CBVSRVUAVAlignment = CBVSRVUAVPool->GetAlignmentSizeInBytes();
    U64 SamplerAlignment = SamplerPool->GetAlignmentSizeInBytes();

    for (U32 I = 0; I < NumDescriptorSets; ++I)
    {
        D3D12DescriptorSet* Set = Malloc<D3D12DescriptorSet>();
        DescriptorSetLayoutInfo& Layout = PLayouts[I];
        U64 SrvSize = static_cast<U64>(Layout.Srv.NumDescriptors);
        U32 CbvSize = static_cast<U64>(Layout.Cbv.NumDescriptors);
        U64 UavSize = static_cast<U64>(Layout.Uav.NumDescriptors);
        U64 SamplerSize = static_cast<U64>(Layout.Sampler.NumDescriptors);
        CBVSRVUAVPool->AllocateDescriptorTable(&Set->TableUpload, 
            (SrvSize + CbvSize + UavSize) * CBVSRVUAVAlignment);
        SamplerPool->AllocateDescriptorTable(&Set->SamplerTableUpload, SamplerSize * SamplerAlignment);
        POutSets[I] = Set; 
    }
    return SResult_OK;
}
} // Synthe