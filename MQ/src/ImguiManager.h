#pragma once

#include <windows.h>
#include "Graphics.h"
#include"Imgui/imgui.h"
#include"Imgui/implot3d.h"
#include"Imgui/imgui_impl_win32.h"
#include"Imgui/imgui_impl_dx11.h"
#include<vector>

class ImGuiManager {
public:
    ImGuiManager(HWND hwnd, Graphics& gfx);
    ~ImGuiManager();

    void BeginFrame(float& x,float& y);
    void EndFrame();

};
