#include "devicemonitor.h"
#include <QDir>
#include <QFileInfo>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

DeviceMonitor::DeviceMonitor(QObject *parent)
    : QObject(parent)
    , m_checkTimer(new QTimer(this))
    , m_cameraInUse(false)
    , m_microphoneInUse(false)
    , m_locationInUse(false)
{
    connect(m_checkTimer, &QTimer::timeout, this, &DeviceMonitor::checkDevices);
    m_checkTimer->start(2000); // 每 2 秒检查一次

    // 初始检查
    checkDevices();
}

void DeviceMonitor::checkDevices()
{
    bool oldCamera = m_cameraInUse;
    bool oldMicrophone = m_microphoneInUse;
    bool oldLocation = m_locationInUse;

    checkCamera();
    checkMicrophone();
    checkLocation();

    if (oldCamera != m_cameraInUse) {
        emit deviceStateChanged(Camera, m_cameraInUse);
    }
    if (oldMicrophone != m_microphoneInUse) {
        emit deviceStateChanged(Microphone, m_microphoneInUse);
    }
    if (oldLocation != m_locationInUse) {
        emit deviceStateChanged(Location, m_locationInUse);
    }
}

void DeviceMonitor::checkCamera()
{
    // Check /dev/video* devices for active access using lsof
    QDir devVideoDir("/dev");
    QStringList videoDevices = devVideoDir.entryList(QStringList("video*"), QDir::System);

    bool inUse = false;
    for (const QString &device : videoDevices) {
        QString devicePath = "/dev/" + device;
        QProcess lsofProcess;
        lsofProcess.start("lsof", QStringList() << "-t" << devicePath);
        lsofProcess.waitForFinished(1000);

        if (lsofProcess.exitCode() == 0 && !lsofProcess.readAll().trimmed().isEmpty()) {
            inUse = true;
            break;
        }
    }

    m_cameraInUse = inUse;
}

void DeviceMonitor::checkMicrophone()
{
    // Check PipeWire/PulseAudio for audio capture activity
    // Using pactl to check if any audio source is in use
    QProcess pactlProcess;
    pactlProcess.start("pactl", QStringList() << "list" << "sources" << "short");
    pactlProcess.waitForFinished(1000);

    QString output = pactlProcess.readAll();
    bool inUse = false;

    if (output.contains("State: RUNNING") || output.contains("CORKED")) {
        inUse = true;
    }

    m_microphoneInUse = inUse;
}

void DeviceMonitor::checkLocation()
{
    // Check Geoclue2 via D-Bus for active clients
    QDBusInterface geoClueManager(
        "org.freedesktop.GeoClue2",
        "/org/freedesktop/GeoClue2/Manager",
        "org.freedesktop.GeoClue2.Manager",
        QDBusConnection::systemBus()
    );

    if (!geoClueManager.isValid()) {
        return;
    }

    // Check if there are any active location requests
    QDBusReply<QDBusObjectPath> reply = geoClueManager.call("GetAgent");
    if (reply.isValid()) {
        m_locationInUse = true;
    } else {
        m_locationInUse = false;
    }
}

// Note: onLocationActiveChanged is not used in polling mode
// This slot exists for future event-based implementation
void DeviceMonitor::onLocationActiveChanged(bool active)
{
    Q_UNUSED(active);
    // Event-based implementation would connect this slot
    // to GeoClue2 D-Bus signals directly
}
