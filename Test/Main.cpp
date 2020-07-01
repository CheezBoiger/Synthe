﻿// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include <iostream>
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/Swapchain.hpp"
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
    CreateInfo.Width = MEM_BYTES(256) * MEM_1MB;
    CreateInfo.Height = 1;
    CreateInfo.DepthOrArraySize = 1;
    CreateInfo.Mips = 1;
    CreateInfo.ResourceFormat = GFormat_UNKNOWN;
    CreateInfo.Usage = ResourceUsage_SHADER_RESOURCE | ResourceUsage_VERTEX_BUFFER;
    CreateInfo.SampleCount = 1;
    CreateInfo.SampleQuality = 0;

    ResourceCreateInfo ImageInfo = { };
    ImageInfo.Width = 1920;
    ImageInfo.Height = 1080;
    ImageInfo.Dimension = ResourceDimension_TEXTURE2D;
    ImageInfo.DepthOrArraySize = 1;
    ImageInfo.Mips = 1;
    ImageInfo.SampleCount = 1;
    ImageInfo.Usage = ResourceUsage_SHADER_RESOURCE | ResourceUsage_RENDER_TARGET;
    ImageInfo.ResourceFormat = GFormat_R8G8B8A8_UNORM;

    ClearValue Clear = { };

    PDevice->CreateResource(&ResourceHandle, &CreateInfo, nullptr);
    CreateInfo.Width = 1024;
    PDevice->CreateResource(&ResourceHandle, &CreateInfo, nullptr);
    PDevice->CreateResource(&ResourceHandle, &ImageInfo, nullptr);

    PDevice->DestroyResource(ResourceHandle);

    U64 MemoryBytes = PDevice->GetCurrentUsedMemoryBytesInPool(MemoryType_TEXTURE) / MEM_1MB;
    GraphicsCommandList* CmdList = nullptr;
    {
        CommandListCreateInfo CmdCi = { };
        CmdCi.Level = CommandListLevel_PRIMARY;
        CmdCi.Type = CommandListType_GRAPHICS;
        CmdCi.Flags = 0;
        PDevice->CreateCommandList(CmdCi, &CmdList);
    }

    Fence* PFence = nullptr;
    PDevice->CreateFence(&PFence);

    CommandListSubmitInfo CmdSi = { };
    CmdSi.NumCommandLists = 1;
    CmdSi.PCmdLists = &CmdList;
    CmdSi.QueueToSubmit = SubmitQueue_GRAPHICS;
    CmdSi.NumSignalFences = 1;
    CmdSi.SignalFences = &PFence;

    while (!PWindow->GetShouldClose()) 
    {
        PollEvents();
        PDevice->Begin();
            GPUHandle BackbufferRTV = PDevice->GetSwapchain()->GetCurrentBackBufferRTV();
            CmdList->Begin();
                CmdList->SetRenderTargets(1, &BackbufferRTV, nullptr);
                ClearColorValue Clear = { 1.0f, 0.0f, 0.0f, 1.0f };
                TargetBounds Bound = { 0, 0, 1920, 1080 };
                CmdList->ClearRenderTarget(BackbufferRTV, &Clear, 1, &Bound);
            CmdList->End();
            PDevice->SubmitCommandLists(1, &CmdSi);
            PDevice->Present();
        PDevice->End();
    }

    PDevice->CleanUp();    
    PWindow->Close();
    PWindow->CleanUp();
    DestroyAppWindow(PWindow);
    return 0;
}