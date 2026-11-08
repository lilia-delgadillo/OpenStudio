#pragma once

#include <QWindow>
#include <memory>

#include "D3D11SwapChain.h"
#include "GraphicsDevice.h"

class VideoSurface final : public QWindow
{
public:
    explicit VideoSurface(GraphicsDevice& graphicsDevice);

protected:
    void exposeEvent(QExposeEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void InitializeSwapChain();
    void Render();

    GraphicsDevice& m_graphicsDevice;
    std::unique_ptr<D3D11SwapChain> m_swapChain;
};