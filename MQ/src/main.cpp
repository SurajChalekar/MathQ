#include "Window.h"
#include "Graphics.h"
#include "Graph2D.h"
#include "ImGuiManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    Window window(hInstance, L"MathQ", 600, 600);
    Graphics gfx(window.GetHwnd(), 600, 600);
	Graph2D graph(gfx);
	ImGuiManager imgui(window.GetHwnd(), gfx);
	float x = 0.0f;
	float y = 0.0f;
    while (window.ProcessMessages()) {

        gfx.Clear(0.1f, 0.2f, 0.3f, 1.0f);
        graph.DrawGrid(8, 8, 0.0f, 0.0f, 0.0f);
		graph.DrawPoint(x, y, 1.0f, 1.0f, 1.0f, 0.05f);
		imgui.BeginFrame(x,y);
		imgui.EndFrame();
        gfx.Present();
    }

    return 0;
}
