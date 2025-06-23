#include "ImGuiManager.h"
#include<math.h>

ImGuiManager::ImGuiManager(HWND hwnd, Graphics& gfx) 

{
    //Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
    ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame() {
   ImGui_ImplDX11_NewFrame();  
   ImGui_ImplWin32_NewFrame();  
   ImGui::NewFrame();  
   ImGui::Begin("Console");
   ImGui::Text("Mandelbrot set");
   ImGui::TextWrapped("The Mandelbrot set is a fascinating mathematical object, a set of points in the complex plane that are connected to a specific iterative equation. It is visually represented as a complex, infinitely detailed fractal, famous for its intricate patterns and self-similar structures when zoomed in. ");
   ImGui::Text(" ");
   ImGui::TextWrapped("It is defined by the iterative equation z = z^2 + c, where z and c are complex numbers. The set consists of all points c for which the sequence does not diverge when starting from z = 0. The boundary of the Mandelbrot set is particularly interesting, exhibiting an infinite variety of shapes and patterns.");
   ImGui::End();
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
