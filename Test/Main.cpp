// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include <iostream>
#include "Graphics/GraphicsDevice.hpp"
#include "Display/Window.hpp"
#include "System.hpp"

using namespace Synthe;

int main(int c, char* argv[])
{
    InitializeSystem();

    Window* PWindow = CreateAppWindow();
    PWindow->Initialize(u8"KITTY CAT  猫の愛 猫の愛 猫の愛 CAt", 0, 0, 1200, 800);
    PWindow->Show();

    GraphicsDevice* PDevice = Synthe::CreateDeviceD3D12();

    GraphicsDeviceConfig GraphicsDeviceConfiguration = { };
    SwapchainConfig SwapchainConfiguration = { };

    GraphicsDeviceConfiguration.DesiredVendor = GPUVendor_NVIDIA;
    GraphicsDeviceConfiguration.DesiredFlags = GraphicsDeviceFlags_CONSERVATIVE_RASTER_BIT;

    SwapchainConfiguration.Count = 3;
    SwapchainConfiguration.Format = GFormat_R8G8B8A8_UNORM;
    SwapchainConfiguration.Height = 800;
    SwapchainConfiguration.Width = 1200;
    SwapchainConfiguration.Windowed = true;
    SwapchainConfiguration.NativeWinHandle = PWindow->GetNativeHandle();

    PDevice->Initialize(GraphicsDeviceConfiguration, SwapchainConfiguration);

    while (!PWindow->GetShouldClose()) 
    {
    }

    PDevice->CleanUp();    
    PWindow->Close();
    PWindow->CleanUp();
    DestroyAppWindow(PWindow);
    return 0;
}