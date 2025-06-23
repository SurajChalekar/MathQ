#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <wrl/client.h>

class Graphics {
public:
    Graphics(HWND hwnd, int width, int height);
    ~Graphics();
	Graphics(const Graphics&) = delete;
    void Resize(int width, int height);
    void Clear(float r, float g, float b, float a);
    void Present();

    ID3D11Device* GetDevice() const { return m_device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};
