#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include "GraphicsDevice.h"

class D3D11Shader final
{
public:
    explicit D3D11Shader(GraphicsDevice& graphicsDevice);
    bool CompileVertexShader(const wchar_t* filePath);
    bool CompilePixelShader(const wchar_t* filePath);

    void Bind();
private:
    GraphicsDevice& m_graphicsDevice;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};