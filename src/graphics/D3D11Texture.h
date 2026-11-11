#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "GraphicsDevice.h"
#include "../engine/video/VideoFrame.h"

class D3D11Texture final
{
public:
    explicit D3D11Texture(GraphicsDevice& graphicsDevice);

    bool Update(const VideoFrame& frame);

    ID3D11ShaderResourceView* GetShaderResourceView() const
    {
        return m_shaderResourceView.Get();
    };

    uint32_t GetWidth() const
    {
        return m_width;
    };
    uint32_t GetHeight() const 
    {
        return m_height;
    };

private:
    bool CreateTexture(const VideoFrame& frame);

    GraphicsDevice& m_graphicsDevice;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

    uint32_t m_width = 0;
    uint32_t m_height = 0;

    VideoFrame::PixelFormat m_format =
        VideoFrame::PixelFormat::BGRA8;
};
