#include "ImGuiManager.h"
#include"RandomWalker.h"
#include<math.h>

ImGuiManager::ImGuiManager(HWND hwnd, Graphics& gfx) 
{
    ImGui_ImplWin32_EnableDpiAwareness();

    //Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot3D::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(gfx.GetDevice(), gfx.GetContext());
}

ImGuiManager::~ImGuiManager() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame(float& x, float& y) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Console");
    ImGui::Text("Classical Walker.");
    ImGui::Text("Current Position: (%.2f, %.2f)", x, y);
    if (ImGui::Button("Next")) {
        std::pair<float, float> n = random_walk_step(x, y); // Reset to a random position
        x = n.first;
        y = n.second;
    }
    ImGui::End();
    ImGui::Render();
}

void ImGuiManager::EndFrame() {

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Multi-viewport support
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
