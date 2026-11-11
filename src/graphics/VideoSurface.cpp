#include "VideoSurface.h"

#include <QExposeEvent>
#include <QResizeEvent>

VideoSurface::VideoSurface(GraphicsDevice& graphicsDevice)
    : m_graphicsDevice(graphicsDevice)
{
    setSurfaceType(QSurface::RasterSurface);
    setWidth(640);
    setHeight(360);
}

void VideoSurface::InitializeSwapChain()
{
    if (m_swapChain)
        return;

    HWND hwnd = reinterpret_cast<HWND>(winId());

    auto swapChain = std::make_unique<D3D11SwapChain>(
        m_graphicsDevice,
        hwnd
    );

    if (!swapChain->Initialize())
        return;

    m_swapChain = std::move(swapChain);

    auto renderer = std::make_unique<D3D11VideoRenderer>(
        m_graphicsDevice
    );

    if (!renderer->Initialize())
        return;

    m_renderer = std::move(renderer);
}

void VideoSurface::Render()
{
    if (!m_swapChain)
        return;

    m_swapChain->Clear(0.0f, 0.0f, 0.0f, 1.0f);

    if (m_renderer)
        m_renderer->Render();

    m_swapChain->Present();
}

void VideoSurface::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    if (!isExposed())
        return;

    InitializeSwapChain();
    Render();
}

void VideoSurface::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);

    if (!m_swapChain)
        return;

    m_swapChain->Resize(width(), height());
    Render();
}

void VideoSurface::UpdateFrame(const VideoFrame& frame)
{
    if (!m_renderer || !m_swapChain)
        return;

    if (!m_renderer->UpdateFrame(frame))
        return;

    Render();
}