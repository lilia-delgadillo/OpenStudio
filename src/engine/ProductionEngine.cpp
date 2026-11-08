#include "ProductionEngine.h"

ProductionEngine::~ProductionEngine()
{
    stop();
}

bool ProductionEngine::setSource(std::unique_ptr<IVideoSource> source)
{
    if (isRunning() || !source)
        return false;

    m_source = std::move(source);
    return true;
}

bool ProductionEngine::start()
{
    return m_source && m_source->start();
}

void ProductionEngine::stop()
{
    if (m_source)
        m_source->stop();
}

bool ProductionEngine::isRunning() const
{
    return m_source && m_source->isRunning();
}

QString ProductionEngine::activeSourceName() const
{
    return m_source ? m_source->name() : QStringLiteral("No source");
}

VideoFrame ProductionEngine::acquireProgramFrame()
{
    if (!isRunning())
        return {};

    return m_source->acquireFrame();
}
