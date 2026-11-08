#include "TestPatternSource.h"

#include <QDateTime>
#include <QPainter>

bool TestPatternSource::start()
{
    m_running = true;
    m_sequenceNumber = 0;
    return true;
}

void TestPatternSource::stop()
{
    m_running = false;
}

bool TestPatternSource::isRunning() const
{
    return m_running;
}

QString TestPatternSource::name() const
{
    return QStringLiteral("Test Pattern");
}

VideoFrame TestPatternSource::acquireFrame()
{
    if (!m_running)
        return {};

    constexpr int width = 1280;
    constexpr int height = 720;

    QImage image(width, height, QImage::Format_RGBA8888);
    image.fill(Qt::black);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // SMPTE-inspired vertical color bars.
    const QList<QColor> bars = {
        QColor(235, 235, 235),
        QColor(235, 235, 0),
        QColor(0, 235, 235),
        QColor(0, 235, 0),
        QColor(235, 0, 235),
        QColor(235, 0, 0),
        QColor(0, 0, 235)
    };

    const int barWidth = width / bars.size();
    for (qsizetype i = 0; i < bars.size(); ++i) {
        painter.fillRect(
            QRect(static_cast<int>(i) * barWidth, 0, barWidth, 500),
            bars[i]);
    }

    painter.fillRect(0, 500, width, 220, QColor(24, 28, 36));

    painter.setPen(Qt::white);
    QFont titleFont = painter.font();
    titleFont.setPixelSize(42);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(QRect(50, 535, 1180, 70),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QStringLiteral("STUDIOCORE  |  TEST SOURCE"));

    QFont detailFont = painter.font();
    detailFont.setPixelSize(22);
    detailFont.setBold(false);
    painter.setFont(detailFont);
    painter.setPen(QColor(190, 200, 210));
    painter.drawText(QRect(50, 610, 1180, 45),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QStringLiteral("1280 x 720  |  Frame %1  |  %2")
                         .arg(m_sequenceNumber)
                         .arg(QDateTime::currentDateTime().toString(
                             QStringLiteral("HH:mm:ss.zzz"))));

    painter.end();

    VideoFrame frame;
    frame.image = std::move(image);
    frame.sequenceNumber = m_sequenceNumber++;
    frame.timestampMs = QDateTime::currentMSecsSinceEpoch();
    return frame;
}
