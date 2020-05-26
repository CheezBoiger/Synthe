// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "D3D12GraphicsDevice.hpp"
#include "D3D12GPUManager.hpp"

#define INTEL_VENDOR_ID 0x8086
#define NVIDIA_VENDOR_ID 0x10DE 
#define NVIDIA_SGS_VENDOR_ID 0x12D2
#define AMD_VENDOR_ID 0x1002
#define AMD_ATI_VENDOR_ID 0x1022
#define QUALCOMM_VENDOR_ID 0x5143
#define MICROSOFT_VENDOR_ID 0x1414


namespace Synthe {


GraphicsDevice* CreateDeviceD3D12()
{
    static D3D12GraphicsDevice Device;
    return &Device;
}

void InitializeHeaps(ID3D12Device* PDevice)
{

    
}

struct DeviceFeatureSupport
{
    D3D12_RAYTRACING_TIER RayTracing;
    D3D12_VARIABLE_SHADING_RATE_TIER VariableShadingRate;
    D3D12_CONSERVATIVE_RASTERIZATION_TIER ConservativeRaster;
    D3D12_TILED_RESOURCES_TIER TiledResources;
    BOOL BarycentricsSupported;
};


struct DeviceInfo
{
    GPUVendor Vendor;
    DeviceFeatureSupport FeatureSupport;
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


GResult D3D12GraphicsDevice::Initialize(const GraphicsDeviceConfig& DeviceConfig, 
                                        const SwapchainConfig& SwapchainConfig)
{
    IDXGIFactory2* PFactory = nullptr;
    HRESULT Result = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&PFactory);
    if (FAILED(Result)) 
    {
        return GResult_INITIALIZATION_FAILURE;    
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

        AdapterDesc.SharedSystemMemory;
        AdapterDesc.DedicatedVideoMemory;
        AdapterDesc.DedicatedSystemMemory;

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

    // Initialize our swapchain.
    m_Swapchain.Initialize(SwapchainConfig, m_Device, PFactory);

    InitializeHeaps(m_Device);    

    m_PFactory = PFactory;
    return GResult_OK;
}

GResult D3D12GraphicsDevice::CleanUp()
{
    m_Swapchain.CleanUp();
    if (m_PFactory)
        m_PFactory->Release();
    if (m_Device)
        m_Device->Release();
    if (m_MLDevice)
        m_MLDevice->Release();
    return GResult_OK;
}
} // Synthe