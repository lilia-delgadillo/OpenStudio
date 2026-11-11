#include "D3D11Shader.h"

/// <summary>
/// Initializes the shader manager with a reference to the shared graphics device.
/// </summary>
/// <param name="graphicsDevice">- The Direct3D 11 graphics device.</param>
D3D11Shader::D3D11Shader(GraphicsDevice& graphicsDevice)
    : m_graphicsDevice(graphicsDevice)
{
}

/// <summary>
/// Compiles the .hlsl shader files for Vertex Shader
/// </summary>
/// <param name="filePath">- Path of the .hlsl file</param>
/// <returns>bool - True if compilation and shader creation succeed; otherwise, false.</returns>
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

/// <summary>
/// Compiles the .hlsl shader files for Pixel Shader
/// </summary>
/// <param name="filePath">- Path of the .hlsl file</param>
/// <returns>bool - True if compilation and shader creation succeed; otherwise, false.</returns>
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

/// <summary>
/// Binds the compiled vertex and pixel shaders to the Direct3D 11 pipeline.
/// </summary>
void D3D11Shader::Bind()
{
    m_graphicsDevice.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_graphicsDevice.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

