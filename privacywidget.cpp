#include "privacywidget.h"
#include "devicemonitor.h"
#include <QPainter>
#include <QApplication>
#include <QSvgRenderer>
#include <QIcon>
#include <QDebug>

PrivacyWidget::PrivacyWidget(QWidget *parent)
    : QWidget(parent)
    , m_cameraInUse(false)
    , m_microphoneInUse(false)
    , m_locationInUse(false)
{
}

void PrivacyWidget::setDeviceStatus(DeviceType type, bool inUse)
{
    switch (type) {
    case Camera:
        m_cameraInUse = inUse;
        break;
    case Microphone:
        m_microphoneInUse = inUse;
        break;
    case Location:
        m_locationInUse = inUse;
        break;
    }

    update();
}

QSize PrivacyWidget::sizeHint() const
{
    return QSize(26, 26);
}

void PrivacyWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // If no devices are in use, show a subtle icon or nothing
    if (!m_cameraInUse && !m_microphoneInUse && !m_locationInUse) {
        // Optional: Show a dimmed icon or empty
        return;
    }

    const int iconSize = qMin(width(), height());
    const int spacing = 4;
    int xOffset = 0;

    // Calculate total width needed
    int activeCount = (m_cameraInUse ? 1 : 0) +
                    (m_microphoneInUse ? 1 : 0) +
                    (m_locationInUse ? 1 : 0);

    if (activeCount == 0) {
        return;
    }

    int totalWidth = activeCount * iconSize + (activeCount - 1) * spacing;
    xOffset = (width() - totalWidth) / 2;

    // Draw camera icon
    if (m_cameraInUse) {
        QPixmap pixmap = loadSvg(":/icons/resources/icons/camera.svg", QSize(iconSize, iconSize));
        painter.drawPixmap(xOffset, (height() - iconSize) / 2, pixmap);
        xOffset += iconSize + spacing;
    }

    // Draw microphone icon
    if (m_microphoneInUse) {
        QPixmap pixmap = loadSvg(":/icons/resources/icons/microphone.svg", QSize(iconSize, iconSize));
        painter.drawPixmap(xOffset, (height() - iconSize) / 2, pixmap);
        xOffset += iconSize + spacing;
    }

    // Draw location icon
    if (m_locationInUse) {
        QPixmap pixmap = loadSvg(":/icons/resources/icons/location.svg", QSize(iconSize, iconSize));
        painter.drawPixmap(xOffset, (height() - iconSize) / 2, pixmap);
    }
}

const QPixmap PrivacyWidget::loadSvg(const QString &fileName, const QSize &size) const
{
    const auto ratio = devicePixelRatioF();
    QPixmap pixmap = QIcon::fromTheme(fileName).pixmap(size * ratio);
    pixmap.setDevicePixelRatio(ratio);
    return pixmap;
}
