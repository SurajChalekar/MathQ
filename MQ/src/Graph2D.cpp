#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Graph2D.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <stdexcept>
#include <cstring>

// Simple vertex structure
struct Vertex {
    float x, y;
    float r, g, b;
};

// HLSL shader source
const char* g_VS = R"(
struct VS_INPUT {
    float2 pos : POSITION;
    float3 color : COLOR;
};
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};
PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.color = input.color;
    return output;
}
)";

const char* g_PS = R"(
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};
float4 main(PS_INPUT input) : SV_TARGET {
    return float4(input.color, 1.0f);
}
)";

Graph2D::Graph2D(Graphics& gfx)
    : m_gfx(gfx)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

    // Compile vertex shader
    HRESULT hr = D3DCompile(g_VS, strlen(g_VS), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr)) throw std::runtime_error("Vertex shader compilation failed");

    // Compile pixel shader
    hr = D3DCompile(g_PS, strlen(g_PS), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr)) throw std::runtime_error("Pixel shader compilation failed");

    // Create shaders
    m_gfx.GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
    m_gfx.GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    m_gfx.GetDevice()->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
}

Graph2D::~Graph2D()
{
    // ComPtrs auto-release
}

void Graph2D::DrawLineGraph(const std::vector<std::pair<float, float>>& points, float r, float g, float b)
{
    if (points.size() < 2) return;

    // Set shaders and input layout
    m_gfx.GetContext()->IASetInputLayout(m_inputLayout.Get());
    m_gfx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_gfx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    std::vector<Vertex> vertices;
    for (const auto& pt : points) {
        vertices.push_back({ pt.first, pt.second, r, g, b });
    }

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_gfx.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    m_gfx.GetContext()->Draw(UINT(vertices.size()), 0);
}
void Graph2D::DrawBarGraph(const std::vector<std::pair<float, float>>& bars, float width, float r, float g, float b)
{
    // Set shaders and input layout
    m_gfx.GetContext()->IASetInputLayout(m_inputLayout.Get());
    m_gfx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_gfx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    for (const auto& bar : bars) {
        float x = bar.first;
        float h = bar.second;
        // Each bar is a vertical line for simplicity
        Vertex verts[2] = {
            { x, 0.0f, r, g, b },
            { x, h,    r, g, b }
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(verts);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = verts;

        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
        HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) continue;

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_gfx.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        m_gfx.GetContext()->Draw(2, 0);
    }
}
void Graph2D::DrawRectangle(float x0, float y0, float x1, float y1, float r, float g, float b)
{
    m_gfx.GetContext()->IASetInputLayout(m_inputLayout.Get());
    m_gfx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_gfx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Correct order for triangle strip:
    // 0: Top-left     (x0, y0)
    // 1: Bottom-left  (x0, y1)
    // 2: Top-right    (x1, y0)
    // 3: Bottom-right (x1, y1)
    Vertex verts[4] = {
        { x0, y0, r, g, b }, // Top-left
        { x0, y1, r, g, b }, // Bottom-left
        { x1, y0, r, g, b }, // Top-right
        { x1, y1, r, g, b }  // Bottom-right
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(verts);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = verts;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_gfx.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    m_gfx.GetContext()->Draw(4, 0);
}
void Graph2D::DrawPixel(float x, float y, float r, float g, float b)
{
    m_gfx.GetContext()->IASetInputLayout(m_inputLayout.Get());
    m_gfx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_gfx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    Vertex vert = { x, y, r, g, b };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(vert);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = &vert;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_gfx.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    m_gfx.GetContext()->Draw(1, 0);
}
void Graph2D::DrawPixels(const std::vector<std::pair<float, float>>& points, float r, float g, float b)
{
    if (points.empty()) return;

    m_gfx.GetContext()->IASetInputLayout(m_inputLayout.Get());
    m_gfx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_gfx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    std::vector<Vertex> vertices;
    vertices.reserve(points.size());
    for (const auto& pt : points) {
        vertices.push_back({ pt.first, pt.second, r, g, b });
    }

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
    if (FAILED(hr)) return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_gfx.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    m_gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    m_gfx.GetContext()->Draw(UINT(vertices.size()), 0);
}

