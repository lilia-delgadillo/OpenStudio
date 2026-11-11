#include "GraphicsDevice.h"

#include <stdexcept>
#include <string>

/// <summary>
/// Creates and initializes the shared Direct3D 11 graphics device and immediate device context.
/// </summary>
/// <exception cref="std::runtime_error">
/// Thrown if Direct3D 11 device initialization fails.
/// </exception>
GraphicsDevice::GraphicsDevice()
{
	D3D_FEATURE_LEVEL featureLevel;

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

    HRESULT result = D3D11CreateDevice(
        nullptr,                    // Default GPU
        D3D_DRIVER_TYPE_HARDWARE,   // Hardware acceleration
        nullptr,                    // No software rasterizer
        0,                          // Creation flags
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(),
        &featureLevel,
        m_context.GetAddressOf()
    );

    if (FAILED(result))
    {
        throw std::runtime_error(
            "Failed to initialize Direct3D 11: " +
            std::to_string(static_cast<unsigned long>(result))
        );
    }
}
// Unimplimented
GraphicsDevice::~GraphicsDevice()
{
}