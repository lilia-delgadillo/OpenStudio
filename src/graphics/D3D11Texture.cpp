#include "D3D11Texture.h"

D3D11Texture::D3D11Texture(GraphicsDevice& graphicsDevice) 
	: m_graphicsDevice(graphicsDevice)
{

}

bool D3D11Texture::Update(const VideoFrame& frame)
{
	if (!frame.isValid())
		return false;

	if (!m_texture ||
		m_width != frame.width ||
		m_height != frame.height ||
		m_format != frame.format)
	{
		if (!CreateTexture(frame))
			return false;
	}

	m_graphicsDevice.GetContext()->UpdateSubresource(
		m_texture.Get(),
		0,
		nullptr,
		frame.pixels.data(),
		frame.stride,
		0
	);

	return true;
}

bool D3D11Texture::CreateTexture(const VideoFrame& frame)
{
	D3D11_TEXTURE2D_DESC description = {};

	description.Width = frame.width;
	description.Height = frame.height;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.SampleDesc.Count = 1;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	switch (frame.format)
	{
	case VideoFrame::PixelFormat::BGRA8:
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;

	case VideoFrame::PixelFormat::BGRX8:
		description.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
		break;

	default:
		return false;
	}

	HRESULT result = m_graphicsDevice.GetDevice()->CreateTexture2D(
		&description,
		nullptr,
		m_texture.GetAddressOf()
	);

	if (FAILED(result))
		return false;

	HRESULT srvResult = m_graphicsDevice.GetDevice()->CreateShaderResourceView(
		m_texture.Get(),
		nullptr,
		m_shaderResourceView.GetAddressOf()
	);

	if (FAILED(srvResult))
		return false;

	m_width = frame.width;
	m_height = frame.height;
	m_format = frame.format;

	return true;
}
