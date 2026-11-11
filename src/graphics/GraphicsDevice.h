#pragma once

#include <d3d11.h>
#include <wrl/client.h>

/// <summary>
/// Manages the Shared Graphics Device
/// </summary>
class GraphicsDevice
{
public:
	GraphicsDevice();
	~GraphicsDevice();

	ID3D11Device* GetDevice() const
	{
		return m_device.Get();
	}

	ID3D11DeviceContext* GetContext() const
	{
		return m_context.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
};