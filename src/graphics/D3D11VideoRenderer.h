#include "GraphicsDevice.h"
#include "../engine/video/VideoFrame.h"

#include "D3D11Shader.h"
#include "D3D11Texture.h"

#include <wrl/client.h>

class D3D11VideoRenderer
{
public:
    explicit D3D11VideoRenderer(GraphicsDevice& graphicsDevice);

    bool Initialize();
    bool UpdateFrame(const VideoFrame& frame);
    void Render();

private:
    GraphicsDevice& m_graphicsDevice;

    D3D11Texture m_videoTexture;
    D3D11Shader m_shader;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
};