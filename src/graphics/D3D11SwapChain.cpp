#include "D3D11SwapChain.h"
#include "GraphicsDevice.h"

D3D11SwapChain::D3D11SwapChain(GraphicsDevice& graphicsDevice, HWND window)
    : m_graphicsDevice(graphicsDevice),
    m_window(window)
{
}

D3D11SwapChain::~D3D11SwapChain() = default;

bool D3D11SwapChain::Initialize() 
{
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;

    HRESULT result = m_graphicsDevice.GetDevice()->QueryInterface(
        IID_PPV_ARGS(&dxgiDevice)
    );

    if (FAILED(result))
        return false;

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;

    result = dxgiDevice->GetAdapter(&adapter);

    if (FAILED(result))
        return false;

    Microsoft::WRL::ComPtr<IDXGIFactory2> factory;

    result = adapter->GetParent(
        IID_PPV_ARGS(&factory)
    );

    if (FAILED(result))
        return false;

    DXGI_SWAP_CHAIN_DESC1 description = {};
    description.Width = 0;
    description.Height = 0;
    description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    description.BufferCount = 2;
    description.Scaling = DXGI_SCALING_STRETCH;
    description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    description.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    description.Flags = 0;

    result = factory->CreateSwapChainForHwnd(
        m_graphicsDevice.GetDevice(),
        m_window,
        &description,
        nullptr,
        nullptr,
        &m_swapChain
    );

    if (FAILED(result))
        return false;

    factory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER);

    if (!CreateRenderTarget())
        return false;

    return true;
}

bool D3D11SwapChain::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    HRESULT result = m_swapChain->GetBuffer(
        0,
        IID_PPV_ARGS(&backBuffer)
    );

    if (FAILED(result))
        return false;

    result = m_graphicsDevice.GetDevice()->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_renderTarget
    );

    if (FAILED(result))
        return false;

    return true;
}

void D3D11SwapChain::Clear(float r, float g, float b, float a)
{
    auto* context = m_graphicsDevice.GetContext();

    context->OMSetRenderTargets(
        1,
        m_renderTarget.GetAddressOf(),
        nullptr
    );
    RECT rect{};
    GetClientRect(m_window, &rect);

    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(rect.right - rect.left);
    viewport.Height = static_cast<float>(rect.bottom - rect.top);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    float color[4] = { r, g, b, a };

    context->ClearRenderTargetView(
        m_renderTarget.Get(),
        color
    );
}

void D3D11SwapChain::Present() 
{
    HRESULT result = m_swapChain->Present(1, 0);

    if (FAILED(result))
    {
        // We'll add proper error reporting later.
        return;
    }
}

void D3D11SwapChain::Resize(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0)
        return;

    m_graphicsDevice.GetContext()->OMSetRenderTargets(
        0,
        nullptr,
        nullptr
    );

    m_renderTarget.Reset();

    HRESULT result = m_swapChain->ResizeBuffers(
        0,
        width,
        height,
        DXGI_FORMAT_UNKNOWN,
        0
    );

    if (FAILED(result))
        return;

    CreateRenderTarget();
}