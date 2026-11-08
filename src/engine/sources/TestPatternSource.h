#pragma once

#include "IVideoSource.h"

class TestPatternSource final : public IVideoSource
{
public:
    bool start() override;
    void stop() override;
    [[nodiscard]] bool isRunning() const override;
    [[nodiscard]] QString name() const override;
    [[nodiscard]] VideoFrame acquireFrame() override;

private:
    bool m_running = false;
    quint64 m_sequenceNumber = 0;
};
