// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Win32Display.hpp"
#include "Win32WindowCallback.hpp"

namespace Synthe {


U32 Display::kFullscreenWidth = 0;
U32 Display::kFullscreenHeight = 0;



void Win32Window::Initialize(const std::string& Title,
        U32 X, U32 Y, U32 Width, U32 Height)
{
    I32 WideStrCount = MultiByteToWideChar(CP_UTF8, 0, Title.c_str(), Title.size(), 0, 0);
    wchar_t* TitleW = new wchar_t[WideStrCount + 1];
    TitleW[WideStrCount] = L'\0';
    MultiByteToWideChar(CP_UTF8, 0, Title.c_str(), Title.size(), TitleW, WideStrCount);
    
    HWND Handle = CreateWindowExW(NULL, WINDOW_CLASS_NAME, TitleW, 
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
        X, Y, Width, Height, NULL, NULL, GetModuleHandle(NULL), NULL);
    delete[] TitleW;
    if (!Handle) return;
    m_NativeHandle = reinterpret_cast<U64>(Handle);
    m_X = X;
    m_Y = Y;
    m_Width = Width;
    m_Height = Height;
    m_Title = Title;
    m_ShouldClose = false;
}


void Win32Window::CleanUp()
{
    Close();
}


void Win32Window::Close()
{
    if (m_NativeHandle) 
    {
        CloseWindow(reinterpret_cast<HWND>(m_NativeHandle));
        DestroyWindow(reinterpret_cast<HWND>(m_NativeHandle));
        m_ShouldClose = true;
        m_NativeHandle = 0;
    }
}


void Win32Window::Show()
{
    ShowWindow(reinterpret_cast<HWND>(m_NativeHandle), SW_SHOW);    
}
} // Synthe