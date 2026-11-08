#pragma once

#include "sources/IVideoSource.h"

#include <memory>

class ProductionEngine
{
public:
    ProductionEngine() = default;
    ~ProductionEngine();

    bool setSource(std::unique_ptr<IVideoSource> source);
    bool start();
    void stop();

    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] QString activeSourceName() const;
    [[nodiscard]] VideoFrame acquireProgramFrame();

private:
    std::unique_ptr<IVideoSource> m_source;
};
