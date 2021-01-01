// No License, this is entirely open source!
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
        // placement new in order to initialize a c style malloc for a class object.
        new (PAllocs) LinearAllocator();
        PAllocs->GetTotalSizeBytes();
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

int countBits32(int A)
{
    // Count our 32 bits 
    A = ((A & 0xAAAAAAAAA) >> 1) + (A & 0x55555555);
    A = ((A & 0xCCCCCCCC) >> 2) + (A & 0x33333333); 
    A = ((A & 0xF0F0F0F0) >> 4) + (A & 0x0F0F0F0F);
    A = ((A & 0xFF00FF00) >> 8) + (A & 0x00FF00FF);
    A = ((A & 0xFFFF0000) >> 16) + (A & 0x0000FFFF);
    return A;
}

int main(int c, char* argv[])
{
    InitializeSystem();
    MemoryStuff();

    Window* PWindow = CreateAppWindow();
    PWindow->Initialize(u8"KITTY CAT  猫の愛 猫の愛 猫の愛 Cat", 0, 0, 1200, 800);
    PWindow->Show();

    GraphicsDevice* PDevice = Synthe::GetDeviceD3D12();

    GraphicsDeviceConfig GraphicsDeviceConfiguration = { };
    SwapchainConfig SwapchainConfiguration = { };

    GraphicsDeviceConfiguration.DesiredVendor = GPUVendor_NVIDIA;
    GraphicsDeviceConfiguration.DesiredFlags = GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT;
    GraphicsDeviceConfiguration.EnableDeviceDebugLayer = true;
    GraphicsDeviceConfiguration.ScratchPoolMemoryInBytes =      MEM_1KB * MEM_BYTES(1024);
    GraphicsDeviceConfiguration.BufferPoolMemoryInBytes =       MEM_1GB * MEM_BYTES(1);
    GraphicsDeviceConfiguration.RenderTargetPoolMemoryInBytes = MEM_1MB * MEM_BYTES(512);
    GraphicsDeviceConfiguration.UploadPoolMemoryInBytes =       MEM_1KB * MEM_BYTES(512);
    GraphicsDeviceConfiguration.ReadBackPoolMemoryInBytes =     MEM_1KB * MEM_BYTES(64);
    GraphicsDeviceConfiguration.TexturePoolMemoryInBytes =      MEM_1MB * MEM_BYTES(512);
    GraphicsDeviceConfiguration.ShaderResourceMemoryInBytes =   MEM_1MB * MEM_BYTES(256);
    
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
    CreateInfo.Dimension = ResourceDimension_BUFFER;
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

    ShaderResourceViewCreateInfo SrvInfo = { };
    SrvInfo.Dimension = SrvViewDimension_TEXTURE2D;
    SrvInfo.Format = GFormat_R8G8B8A8_UNORM;
    SrvInfo.ResourceHandle = ResourceHandle;
    SrvInfo.Texture2D.MipLevels = 1;
    SrvInfo.Texture2D.MostDetailedMip = 0;
    SrvInfo.Texture2D.PlaneSlice = 0;
    SrvInfo.Texture2D.ResourceMinLODClamp = 0.0f;
    GPUHandle SrvHandle;
    PDevice->CreateShaderResourceView(SrvInfo, &SrvHandle);
    
    RootSignature* RootSig = nullptr;
    DescriptorSet* PSet = nullptr;
    {
        RootSignatureLayoutInfo Ci = { };
        DescriptorSetLayoutInfo DescLayout = { };
        DescLayout.Srv.BaseRegister = 0;
        DescLayout.Srv.NumDescriptors = 1;
        Ci.LayoutInfos = &DescLayout;
        Ci.NumDescriptorTables = 1;
        PDevice->CreateRootSignature(&RootSig, Ci);
        PDevice->AllocateDescriptorSets(1, &PSet, &DescLayout);
    }

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

    RootSignature* PRootSig = nullptr;
    Fence* PFence = nullptr;
    PDevice->CreateFence(&PFence);

    CommandListSubmitInfo CmdSi = { };
    CmdSi.NumCommandLists = 1;
    CmdSi.PCmdLists = &CmdList;
    CmdSi.QueueToSubmit = SubmitQueue_GRAPHICS;
    CmdSi.NumSignalFences = 1;
    CmdSi.SignalFences = &PFence;
    
    {
        RootSignatureLayoutInfo RSCreateInfo = { };
        DescriptorSetLayoutInfo Layout = { };
        Layout.Srv.NumDescriptors = 2;
        Layout.Srv.BaseRegister = 0;
        RSCreateInfo.NumDescriptorTables = 1;
        RSCreateInfo.LayoutInfos = &Layout;
        PDevice->CreateRootSignature(&PRootSig, RSCreateInfo);
    }
    
    R32 C = 0.f;
    while (!PWindow->GetShouldClose()) 
    {
        PollEvents();
    
        // Render Begin Block.
        PDevice->Begin();
            GPUHandle BackbufferRTV = PDevice->GetSwapchain()->GetCurrentBackBufferRTV();
            CmdList->Begin();
                CmdList->SetRenderTargets(1, &BackbufferRTV, nullptr);
                ClearColorValue Clear = { sinf((C = C + 0.01f)), cosf(C), 0.0f, 1.0f };
                TargetBounds Bound = { 0, 0, 1920, 1080 };
                CmdList->ClearRenderTarget(BackbufferRTV, &Clear, 1, &Bound);
                CmdList->SetPipelineState(PipelineStateType_GRAPHICS, nullptr, PRootSig);
                CmdList->DrawInstanced(3, 1, 0, 0);
            CmdList->End();
            PDevice->SubmitCommandLists(1, &CmdSi);
            PDevice->Present();
        // Render End Block.
        PDevice->End();
    }

    PDevice->CleanUp();    
    PWindow->Close();
    PWindow->CleanUp();
    DestroyAppWindow(PWindow);
    return 0;
}