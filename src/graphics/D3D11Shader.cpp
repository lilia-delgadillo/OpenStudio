#include "D3D11Shader.h"

D3D11Shader::D3D11Shader(GraphicsDevice& graphicsDevice)
    : m_graphicsDevice(graphicsDevice)
{
}

bool D3D11Shader::CompileVertexShader(const wchar_t* filePath)
{
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT result = D3DCompileFromFile(
        filePath,
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    
    if (FAILED(result))
        return false;

    result = m_graphicsDevice.GetDevice()->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        m_vertexShader.GetAddressOf()
    );

    if (FAILED(result))
        return false;

    return true;
}

bool D3D11Shader::CompilePixelShader(const wchar_t* filePath)
{
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT result = D3DCompileFromFile(
        filePath,
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(result))
        return false;

    result = m_graphicsDevice.GetDevice()->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.GetAddressOf()
    );

    if (FAILED(result))
        return false;

    return true;
}

void D3D11Shader::Bind()
{
    m_graphicsDevice.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_graphicsDevice.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

