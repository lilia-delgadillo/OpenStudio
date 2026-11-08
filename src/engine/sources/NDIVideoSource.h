#pragma once

#include "../video/VideoFrame.h"
#include "./IVideoSource.h"

#include <QString>
#include "../video/NDI/NDIMain.h"

class NDIVideoSource : public IVideoSource
{
public:
    bool start() override;
    void stop() override;
    [[nodiscard]] bool isRunning() const override;
    [[nodiscard]] QString name() const override;
    [[nodiscard]] VideoFrame acquireFrame() override;

    void startNDIReciver(NDIlib_source_t source);

private:
    bool m_running = false;
    NDIlib_recv_instance_t reciver;
    quint64 m_sequenceNumber = 0;

};
