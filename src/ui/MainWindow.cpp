#include "MainWindow.h"

#include "../engine/sources/NDIVideoSource.h"

#include <QLabel>
#include <QPainter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

#include <chrono>

class VideoCanvas final : public QWidget
{
public:
    explicit VideoCanvas(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setMinimumSize(640, 360);
        setStyleSheet(QStringLiteral("background-color: #101318;"));
    }

    void setFrame(const QImage& frame)
    {
        m_frame = frame;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(12, 15, 20));

        if (m_frame.isNull()) {
            painter.setPen(Qt::white);
            painter.drawText(rect(), Qt::AlignCenter,
                             QStringLiteral("Waiting for video..."));
            return;
        }

        const QSize scaledSize = m_frame.size().scaled(
            size(), Qt::KeepAspectRatio);
        const QRect target(
            (width() - scaledSize.width()) / 2,
            (height() - scaledSize.height()) / 2,
            scaledSize.width(),
            scaledSize.height());

        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawImage(target, m_frame);
    }

private:
    QImage m_frame;
};

MainWindow::MainWindow(GraphicsDevice& graphicsDevice,
    QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("StudioCore V0.1"));
    resize(1100, 720);

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* header = new QLabel(
        QStringLiteral("STUDIOCORE  /  PROGRAM OUTPUT"), central);
    header->setStyleSheet(
        QStringLiteral("font-size: 18px; font-weight: 700; "
                       "color: #f2f4f8; padding: 8px;"));

    m_programCanvas = new VideoSurface(graphicsDevice);

    QWidget* videoContainer =
        QWidget::createWindowContainer(m_programCanvas, central);

    videoContainer->setMinimumSize(640, 360);

    layout->addWidget(header);
    layout->addWidget(videoContainer, 1);
    central->setStyleSheet(QStringLiteral("background-color: #202630;"));
    setCentralWidget(central);

    m_engine.setSource(std::make_unique<NDIVideoSource>());

    if (!m_engine.start()) {
        statusBar()->showMessage(
            QStringLiteral("Failed to start the test source."));
    } else {
        statusBar()->showMessage(
            QStringLiteral("Program source: %1 | FPS: %2")
                .arg(m_engine.activeSourceName())
                .arg(0));
    }

    connect(&m_frameTimer, &QTimer::timeout,
            this, &MainWindow::updateFrame);
 
    m_frameTimer.start(15); // UI refresh target ~30 Hz for this first checkpoint.
}

MainWindow::~MainWindow()
{
    m_frameTimer.stop();
    m_engine.stop();
}

void MainWindow::updateFrame()
{
    // 1. Grab the frame from the engine
    const VideoFrame frame = m_engine.acquireProgramFrame();

    // 2. Only process and calculate FPS if we actually got a valid new frame
    if (frame.isValid()) {

        auto now = std::chrono::steady_clock::now();

        if (m_isFirstFrame) {
            // First frame initialization to prevent massive spike on frame 1
            m_lastFrameTime = now;
            m_isFirstFrame = false;

            statusBar()->showMessage(
                QStringLiteral("Program source: %1 | FPS: Calculating...")
                .arg(m_engine.activeSourceName()));
        }
        else {
            // 3. Calculate time elapsed since the PREVIOUS frame arrived
            auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastFrameTime).count();

            // Keep the timestamp moving forward
            m_lastFrameTime = now;

            // 4. Calculate actual FPS (prevent division by zero)
            double elapsed_secs = (elapsed_us > 0) ? (elapsed_us / 1000000.0) : 0.000001;
            double fps = 1.0 / elapsed_secs;

            // 5. Update the UI
            statusBar()->showMessage(
                QStringLiteral("Program source: %1 | FPS: %2")
                .arg(m_engine.activeSourceName())
                .arg(QString::number(fps, 'f', 1))); // Formats to 1 decimal place
        }
    }
}
