#include "D3D11VideoRenderer.h"

D3D11VideoRenderer::D3D11VideoRenderer(GraphicsDevice& graphicsDevice)
    : m_graphicsDevice(graphicsDevice),
    m_videoTexture(graphicsDevice),
    m_shader(graphicsDevice)
{
}

bool D3D11VideoRenderer::Initialize()
{
    bool result = m_shader.CompileVertexShader(L"shaders/VideoVertexShader.hlsl");

    if (!result)
        return false;

    result = m_shader.CompilePixelShader(L"shaders/VideoPixelShader.hlsl");

    if (!result)
        return false;

    D3D11_SAMPLER_DESC samplerDesc = {};

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT result_hr = m_graphicsDevice.GetDevice()->CreateSamplerState(
        &samplerDesc,
        m_samplerState.GetAddressOf()
    );

    if (FAILED(result_hr))
        return false;

    return true;
}

bool D3D11VideoRenderer::UpdateFrame(const VideoFrame& frame)
{
    if (!frame.isValid())
        return false;

    return m_videoTexture.Update(frame);
}

void D3D11VideoRenderer::Render()
{
    m_shader.Bind();

    ID3D11ShaderResourceView* textureView = m_videoTexture.GetShaderResourceView();
    ID3D11SamplerState* sampler = m_samplerState.Get();

    m_graphicsDevice.GetContext()->PSSetShaderResources(
        0,
        1,
        &textureView
    );

    m_graphicsDevice.GetContext()->PSSetSamplers(
        0,
        1,
        &sampler
    );

    m_graphicsDevice.GetContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    m_graphicsDevice.GetContext()->Draw(3, 0);
}