#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

struct VideoFrame
{
    enum class PixelFormat : uint8_t{
        BGRA8,
        BGRX8
    };

    std::vector<uint8_t> pixels;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t stride = 0;
    PixelFormat format = PixelFormat::BGRA8;
    uint64_t sequenceNumber = 0;

    int64_t timestampMs = 0;

    [[nodiscard]] bool isValid() const noexcept
    {
        if (width == 0 || height == 0 || stride == 0)
            return false;

        const std::size_t requiredBytes =
            static_cast<std::size_t>(stride) * height;

        return pixels.size() >= requiredBytes;
    }
};
