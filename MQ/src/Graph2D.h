#pragma once

#include "Graphics.h"
#include <vector>
#include <utility> // for std::pair

class Graph2D {
public:
    Graph2D(Graphics& gfx);
    ~Graph2D();
    void DrawPoint(float x, float y, float r, float g, float b, float size = 0.02f)
    {
        float half = size * 0.5f;
        // Draw a filled rectangle centered at (x, y)
        DrawRectangle(x - half, y - half, x + half, y + half, r, g, b);
    }
    void DrawGrid(int nx, int ny, float r, float g, float b);
    // Draw a line graph. Points are in normalized device coordinates (-1..1)
    void DrawLineGraph(const std::vector<std::pair<float, float>>& points, float r, float g, float b);

    // Draw a bar graph. Each bar is a pair (center x, height), width is fixed.
    void DrawBarGraph(const std::vector<std::pair<float, float>>& bars, float width, float r, float g, float b);
    void DrawRectangle(float x0, float y0, float x1, float y1, float r, float g, float b);
    void DrawPixel(float x, float y, float r, float g, float b);
    void DrawPixels(const std::vector<std::pair<float, float>>& points, float r, float g, float b);

private:
    Graphics& m_gfx;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
