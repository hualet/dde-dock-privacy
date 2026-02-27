#ifndef PRIVACYWIDGET_H
#define PRIVACYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "devicemonitor.h"

class PrivacyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrivacyWidget(QWidget *parent = nullptr);

    void setDeviceStatus(DeviceType type, bool inUse);

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *e) override;

private:
    const QPixmap loadSvg(const QString &fileName, const QSize &size) const;

private:
    bool m_cameraInUse;
    bool m_microphoneInUse;
    bool m_locationInUse;
};

#endif // PRIVACYWIDGET_H
