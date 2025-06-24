#include "ImGuiManager.h"
#include<math.h>

ImGuiManager::ImGuiManager(HWND hwnd, Graphics& gfx) 

{
    //Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot3D::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(gfx.GetDevice(), gfx.GetContext());
}

ImGuiManager::~ImGuiManager() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame() {
   ImGui_ImplDX11_NewFrame();  
   ImGui_ImplWin32_NewFrame();  
   ImGui::NewFrame();
   ImGui::ShowDemoWindow();
   ImPlot3D::ShowDemoWindow();

   // Fix: Use the ID of the main viewport instead of the viewport pointer  
   ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);  
}

void ImGuiManager::EndFrame() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Multi-viewport support
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
