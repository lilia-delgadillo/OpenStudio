#pragma once

#include <d3d11.h>       // Direct3D 11 interfaces
#include <dxgi1_2.h>     // DXGI swap chain interfaces
#include <wrl/client.h>  // Microsoft::WRL::ComPtr
#include <Windows.h>     // HWND and Windows types

class GraphicsDevice;

class D3D11SwapChain
{
public:
	D3D11SwapChain(GraphicsDevice& graphicsDevice, HWND window);
    ~D3D11SwapChain();

    bool Initialize();
    void Resize(unsigned int width, unsigned int height);
    void Clear(float r, float g, float b, float a);
    void Present();

private:
    bool CreateRenderTarget();

    GraphicsDevice& m_graphicsDevice;
    HWND m_window = nullptr;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
};