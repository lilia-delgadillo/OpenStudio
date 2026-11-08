#pragma once

#include "../engine/ProductionEngine.h"

#include <QMainWindow>
#include <QTimer>

#include <chrono>

class VideoCanvas;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void updateFrame();

private:
    ProductionEngine m_engine;
    QTimer m_frameTimer;
    VideoCanvas* m_programCanvas = nullptr;

    std::chrono::steady_clock::time_point m_lastFrameTime;
    bool m_isFirstFrame = true;

};
