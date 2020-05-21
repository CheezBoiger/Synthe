// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Display/Window.hpp"

namespace Synthe {

void InitializeWin32Windows();

class Display
{
public:
    static U32 kFullscreenWidth;
    static U32 kFullscreenHeight;
};



class Win32Window : public Window
{
public:
    virtual ~Win32Window() { }
    virtual void Initialize(const std::string& Title,
        U32 X, U32 Y, U32 Width, U32 Height) override;
    virtual void CleanUp() override;
    virtual void Close() override;
    virtual void Show() override;
};
} // Synthe