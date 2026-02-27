#ifndef DEVICEMONITOR_H
#define DEVICEMONITOR_H

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QDBusConnection>

enum DeviceType {
    Camera,
    Microphone,
    Location
};

Q_DECLARE_METATYPE(DeviceType)

class DeviceMonitor : public QObject
{
    Q_OBJECT

public:
    explicit DeviceMonitor(QObject *parent = nullptr);

    bool isCameraInUse() const { return m_cameraInUse; }
    bool isMicrophoneInUse() const { return m_microphoneInUse; }
    bool isLocationInUse() const { return m_locationInUse; }

signals:
    void deviceStateChanged(DeviceType type, bool inUse);

private slots:
    void checkDevices();
    void onLocationActiveChanged(bool active);

private:
    void checkCamera();
    void checkMicrophone();
    void checkLocation();

private:
    QTimer *m_checkTimer;
    bool m_cameraInUse;
    bool m_microphoneInUse;
    bool m_locationInUse;
};

#endif // DEVICEMONITOR_H
