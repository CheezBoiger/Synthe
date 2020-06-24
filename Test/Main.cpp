// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include <iostream>
#include "Graphics/GraphicsDevice.hpp"
#include "Common/Memory/LinearAllocator.hpp"

#include "Display/Window.hpp"
#include "System.hpp"

using namespace Synthe;

void MemoryStuff()
{
    U64 TotalSizeBytes = MEM_1MB * MEM_BYTES(2);
    void* MemoryBlock = MallocArray<MemT>(TotalSizeBytes);   
    Allocator* PAllocator = Malloc<LinearAllocator>();
    PAllocator->Initialize(reinterpret_cast<Allocator::UPtr>(MemoryBlock), TotalSizeBytes);

    float* A = nullptr;
    int* B = nullptr;
    char* SArray = nullptr;
    Allocator* PAllocs = nullptr;

    {
        AllocationBlock Block = { };
        PAllocator->Allocate(&Block, sizeof(float), MEM_BYTES(4));
        A = reinterpret_cast<float*>(Block.StartAddress);
    }

    {
        AllocationBlock Block = { };
        PAllocator->Allocate(&Block, sizeof(char) * 32ULL, MEM_BYTES(4));
        SArray = reinterpret_cast<char*>(Block.StartAddress);
    }

    {
        AllocationBlock Block = { };
        PAllocator->Allocate(&Block, sizeof(int), MEM_BYTES(4));
        B = reinterpret_cast<int*>(Block.StartAddress);
    }
    
    {
        AllocationBlock Block = { };
        PAllocator->Allocate(&Block, sizeof(LinearAllocator) * MEM_BYTES(32), MEM_BYTES(4));
        PAllocs = reinterpret_cast<LinearAllocator*>(Block.StartAddress);
    }

    *A = 512.12345f;
    *B = -32;
    SArray[0] = 'C'; SArray[1] = 'A'; SArray[2] = 'T';
    
    PAllocator->Reset();
    
    {
        AllocationBlock Block = { };
        PAllocator->Allocate(&Block, sizeof(double), MEM_BYTES(8));
        double* Cat = reinterpret_cast<double*>(Block.StartAddress);
        *Cat = -9123124.14329424;
        *Cat = 12.;
    }

    Free(PAllocator);
    FreeArray(MemoryBlock);
}

int main(int c, char* argv[])
{
    InitializeSystem();
    MemoryStuff();

    Window* PWindow = CreateAppWindow();
    PWindow->Initialize(u8"KITTY CAT  猫の愛 猫の愛 猫の愛 Cat", 0, 0, 1200, 800);
    PWindow->Show();

    GraphicsDevice* PDevice = Synthe::CreateDeviceD3D12();

    GraphicsDeviceConfig GraphicsDeviceConfiguration = { };
    SwapchainConfig SwapchainConfiguration = { };

    GraphicsDeviceConfiguration.DesiredVendor = GPUVendor_NVIDIA;
    GraphicsDeviceConfiguration.DesiredFlags = GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT;
    GraphicsDeviceConfiguration.EnableDeviceDebugLayer = true;
    
    SwapchainConfiguration.NumFrames = 3;
    SwapchainConfiguration.Buffering = 3;
    SwapchainConfiguration.Format = GFormat_R8G8B8A8_UNORM;
    SwapchainConfiguration.Height = 800;
    SwapchainConfiguration.Width = 1200;
    SwapchainConfiguration.Windowed = true;
    SwapchainConfiguration.NativeWinHandle = PWindow->GetNativeHandle();

    PDevice->Initialize(GraphicsDeviceConfiguration, SwapchainConfiguration);

    GPUHandle ResourceHandle = 0;
    ResourceCreateInfo CreateInfo = { };
    CreateInfo.Width = 256;
    CreateInfo.Height = 1;
    CreateInfo.DepthOrArraySize = 1;
    CreateInfo.Mips = 1;
    CreateInfo.ResourceFormat = GFormat_UNKNOWN;
    CreateInfo.Usage = ResourceUsage_SHADER_RESOURCE | ResourceUsage_VERTEX_BUFFER;
    CreateInfo.SampleCount = 1;
    CreateInfo.SampleQuality = 0;
    
    PDevice->CreateResource(&ResourceHandle, &CreateInfo);
    CreateInfo.Width = 1024;
    PDevice->CreateResource(&ResourceHandle, &CreateInfo);

    PDevice->DestroyResource(ResourceHandle);

    while (!PWindow->GetShouldClose()) 
    {
        PollEvents();
    }

    PDevice->CleanUp();    
    PWindow->Close();
    PWindow->CleanUp();
    DestroyAppWindow(PWindow);
    return 0;
}