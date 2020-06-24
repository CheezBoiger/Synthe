// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia

#include "System.hpp"
#include "Win32Common.hpp"
#include "Win32Display.hpp"
#include "Win32WindowCallback.hpp"


namespace Synthe {


LRESULT NativeWindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    Window* PWindow = reinterpret_cast<Window*>(GetPropW(Hwnd, WINDOW_PROP_NAME));
    if (!PWindow) 
    {
        // Can't do much with the window, but the handle must still exist.
    }

    switch (UMsg) 
    {
        case WM_KEYDOWN:
            break;
        case WM_KEYUP:
            break;
        case WM_SHOWWINDOW:
            break;
        case WM_MOVE:
            break;
        case WM_MOUSEMOVE:
            break;
        case WM_CLOSE:
        case WM_QUIT:
        {
            if (PWindow)
            {
                PWindow->Close();
            }
            break;
        }
    }

    return DefWindowProcW(Hwnd, UMsg, WParam, LParam);
}
} // Synthe