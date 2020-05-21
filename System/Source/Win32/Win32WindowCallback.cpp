// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "System.hpp"
#include "Win32Common.hpp"
#include "Win32Display.hpp"
#include "Win32WindowCallback.hpp"


namespace Synthe {

void InitializeSystem()
{
    InitializeWin32Windows();

}

void InitializeWin32Windows()
{
    WNDCLASSEXW WindowClassEx = { };
    WindowClassEx.cbSize = sizeof(WNDCLASSEXW);
    WindowClassEx.lpfnWndProc = NativeWindowProc;
    WindowClassEx.lpszClassName = WINDOW_CLASS_NAME;
    WindowClassEx.hInstance = GetModuleHandle(NULL);
    WindowClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClassEx.hIcon = LoadIcon(NULL, IDC_ARROW);
    WindowClassEx.hCursor = LoadCursor(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&WindowClassEx)) 
    {
        // Failed. Window Register.
        return;
    }
    
    Display::kFullscreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    Display::kFullscreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
}


Window* CreateAppWindow()
{
    return new Win32Window();
}


void DestroyAppWindow(Window* PWindow)
{
    Win32Window* PWin32Window = static_cast<Win32Window*>(PWindow);
    if (PWin32Window->GetShouldClose())
    {
        PWin32Window->Close();
    }
    delete PWin32Window;
}
} // Synthe
