#pragma once

#include <windows.h>
#include <string>
#include <functional>

class Window{
public:
    Window(HINSTANCE hInstance, const std::wstring& windowTitle, int width = 800, int height = 600);
    ~Window();

    HWND GetHwnd() const { return m_hwnd; }
    bool ProcessMessages();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void RegisterWindowClass();
    void CreateAppWindow(int width, int height);

private:
    HINSTANCE m_hInstance;
    std::wstring m_className = L"My Window";
    std::wstring m_windowTitle;
    HWND m_hwnd;
};