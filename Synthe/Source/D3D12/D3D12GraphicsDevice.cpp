// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GraphicsDevice.hpp"
#include "D3D12DescriptorManager.hpp"
#include "D3D12MemoryManager.hpp"

namespace Synthe {


GPUHandle KGPUHandleAssign = 0ULL;


GPUHandle GenerateNewHandle()
{
    return ++KGPUHandleAssign; 
}


GraphicsDevice* CreateDeviceD3D12()
{
    static D3D12GraphicsDevice Device;
    return &Device;
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
}

void InitializeDescriptorHeaps(ID3D12Device* PDevice, U32 BufferingCount)
{
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_CBV_SRV_UAV, BufferingCount);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_CBV_SRV_UAV_UPLOAD, 1);
    
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_SAMPLER, BufferingCount);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_SAMPLER_UPLOAD, 1);

    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_RTV, 1U);
    D3D12DescriptorManager::CreateAndRegisterDescriptorPools(DescriptorType_DSV, 1U);
    
    // Resource creations.
    D3D12DescriptorManager::GetDescriptorPool(DescriptorType_RTV)->Create(PDevice, 
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 512);
    D3D12DescriptorManager::GetDescriptorPool(DescriptorType_DSV)->Create(PDevice,
        D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 2048);
    for (U32 I = 0; I < BufferingCount; ++I)
    {
        D3D12DescriptorManager::GetDescriptorPool(DescriptorType_CBV_SRV_UAV, I)->Create(PDevice, 
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 8192);
        D3D12DescriptorManager::GetDescriptorPool(DescriptorType_SAMPLER, I)->Create(PDevice,
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

    m_Features.Vendor = BestInfo.Vendor;
    m_Features.SharedSystemMemory = BestInfo.SharedMemoryBytes;
    m_Features.DedicatedSystemMemoryInBytes = BestInfo.DedicatedSystemMemoryBytes;
    m_Features.DedicatedVideoMemoryInBytes = BestInfo.DedicatedVideoMemoryBytes;
    m_ResourceHeapTier = BestInfo.FeatureSupport.ResourceHeapTier;

    InitializeMemoryHeaps(m_Device);
    InitializeDescriptorHeaps(m_Device, SwapchainConfig.Buffering);    
    CreateBackbufferQueue();
    // Initialize our swapchain.
    m_Swapchain.Initialize(SwapchainConfig, m_BackbufferQueue, PFactory);
    // Initialize RTVs for swapchain.
    m_Swapchain.BuildRTVs(m_Device, D3D12DescriptorManager::GetDescriptorPool(DescriptorType_RTV));

    QueryBufferingResources(SwapchainConfig.Buffering);

    m_PFactory = PFactory;
    return SResult_OK;
}


void D3D12GraphicsDevice::SubmitCommandListsToBackBuffer(ID3D12CommandList* const* PPCommandLists, U32 Count, U32 FrameIndex)
{
    BufferingResource& Buffer = m_BufferingResources[FrameIndex % m_BufferingResources.size()];
    m_BackbufferQueue->Wait(Buffer.PWaitFence, Buffer.FenceSignalValue);
    m_BackbufferQueue->ExecuteCommandLists(Count, PPCommandLists);
    U32 FenceNewValue = Buffer.FenceSignalValue++;
    m_BackbufferQueue->Signal(Buffer.PSignalFence, FenceNewValue);
}


ResultCode D3D12GraphicsDevice::CleanUp()
{
    m_Swapchain.CleanUp();
    if (m_PFactory)
        m_PFactory->Release();
    if (m_Device)
        m_Device->Release();
    if (m_MLDevice)
        m_MLDevice->Release();
    return SResult_OK;
}


ResultCode D3D12GraphicsDevice::CreateBackbufferQueue()
{
    D3D12_COMMAND_QUEUE_DESC Desc = { };
    Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    Desc.NodeMask = 0;
    Desc.Priority = 0;
    Desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    HRESULT HR = m_Device->CreateCommandQueue(&Desc, __uuidof(ID3D12CommandQueue), (void**)&m_BackbufferQueue);
    if (FAILED(HR)) 
        return GResult_DEVICE_CREATION_FAILURE;
    return SResult_OK;
}


void D3D12GraphicsDevice::CleanUpBufferingResources()
{
    m_BackBufferCommandList.Release();
    for (U32 I = 0; I < m_BufferingResources.size(); ++I) 
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        Buffer.PWaitFence->Release();
        Buffer.PSignalFence->Release();
        CloseHandle(Buffer.FenceEventSignal);
        CloseHandle(Buffer.FenceEventWait);
        Buffer.PCommandAllocator->Release();
    }  
}


void D3D12GraphicsDevice::QueryBufferingResources(U32 BufferingCount)
{
    CleanUpBufferingResources();
    m_BufferingResources.resize(BufferingCount);
    std::vector<ID3D12CommandAllocator*> PPerBufferAllocators(BufferingCount);
    for (U32 I = 0; I < m_BufferingResources.size(); ++I)
    {
        BufferingResource& Buffer = m_BufferingResources[I];
        
        Buffer.FenceEventSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
        Buffer.FenceEventWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    
        HRESULT Result = 0; 
        m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Buffer.PSignalFence);
        m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Buffer.PWaitFence);
        m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&Buffer.PCommandAllocator);
        PPerBufferAllocators[I] = Buffer.PCommandAllocator;
    }
    m_BackBufferCommandList.Initialize(m_Device, BufferingCount, PPerBufferAllocators.data(), 
        D3D12_COMMAND_LIST_TYPE_DIRECT);
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
    return m_Swapchain.Present();
}


void D3D12GraphicsDevice::Begin()
{    
    // Next frame to work on.
    m_BufferIndex = m_Swapchain.GetNextFrameIndex() % static_cast<U32>(m_BufferingResources.size());

    BufferingResource& Buffer = m_BufferingResources[m_BufferIndex];
    Buffer.PCommandAllocator->Reset();

    // Submit to next buffer index.
    m_BackBufferCommandList.SetCurrentIdx(m_BufferIndex);
    m_BackBufferCommandList.Reset();
}


void D3D12GraphicsDevice::End()
{
    BufferingResource& Buffer = m_BufferingResources[m_BufferIndex];
    m_BackbufferQueue->Signal(Buffer.PSignalFence, Buffer.FenceSignalValue);
    
}


ResultCode D3D12GraphicsDevice::CreateResource(GPUHandle* Out, const ResourceCreateInfo* PCreateInfo)
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
    D3D12_RESOURCE_STATES InitialState = D3D12_RESOURCE_STATE_COMMON;

    ResultCode Result = D3D12MemoryManager::GetMemoryPool(MemType)->AllocateResource(m_Device, 
        ResourceDesc, InitialState, 
        ResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER ? nullptr : &ClearValue, 
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
} // Synthe