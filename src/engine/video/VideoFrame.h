#pragma once

#include <QImage>
#include <QtGlobal>

struct VideoFrame
{
    QImage image;
    quint64 sequenceNumber = 0;
    qint64 timestampMs = 0;

    [[nodiscard]] bool isValid() const noexcept
    {
        return !image.isNull();
    }
};
