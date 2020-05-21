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
    switch (UMsg) {
    }

    return DefWindowProcW(Hwnd, UMsg, WParam, LParam);
}
} // Synthe