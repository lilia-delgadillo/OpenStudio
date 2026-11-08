#pragma once

#include "../video/VideoFrame.h"

#include <QString>

class IVideoSource
{
public:
    virtual ~IVideoSource() = default;

    virtual bool start() = 0;
    virtual void stop() = 0;
    [[nodiscard]] virtual bool isRunning() const = 0;
    [[nodiscard]] virtual QString name() const = 0;
    [[nodiscard]] virtual VideoFrame acquireFrame() = 0;
};
