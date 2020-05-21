// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Win32Common.hpp"
#include "Common/Types.hpp"

#define WINDOW_CLASS_NAME L"SystemSyntheWindowClass"

namespace Synthe {


LRESULT NativeWindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam);
} // Synthe