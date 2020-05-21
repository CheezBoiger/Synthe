// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"

namespace Synthe {


class Window {
public:
    virtual ~Window() { }

    virtual void Initialize(const std::string& Title,
        U32 X, U32 Y, U32 Width, U32 Height) = 0;
    virtual void CleanUp() = 0;
    virtual void Close() = 0;
    virtual void Show() = 0;
    const std::string& GetTitleName() const { return m_Title; }
    U32 GetX() const { return m_X; }
    U32 GetY() const { return m_Y; }
    U32 GetWidth() const { return m_Width; }
    U32 GetHeight() const { return m_Height; }

    U64 GetNativeHandle() const { return m_NativeHandle; }
    B32 GetShouldClose() const { return m_ShouldClose; }
    
protected:
    std::string m_Title;
    U32 m_X, m_Y, m_Width, m_Height;
    U64 m_NativeHandle;
    B32 m_ShouldClose;
};
} // Synthe