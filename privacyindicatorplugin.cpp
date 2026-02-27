#include "privacyindicatorplugin.h"
#include <QDebug>

#define PLUGIN_STATE_KEY "enable"

PrivacyIndicatorPlugin::PrivacyIndicatorPlugin(QObject *parent)
    : QObject(parent)
    , m_pluginLoaded(false)
    , m_tipsWidget(nullptr)
    , m_deviceMonitor(nullptr)
{
    qDebug() << "[PrivacyIndicator] Plugin constructor called";
}

const QString PrivacyIndicatorPlugin::pluginName() const
{
    return "privacy-indicator";
}

const QString PrivacyIndicatorPlugin::pluginDisplayName() const
{
    return QObject::tr("隐私指示器");
}

QWidget *PrivacyIndicatorPlugin::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);
    qDebug() << "[PrivacyIndicator] itemWidget() called, returning widget:" << m_pluginWidget.data();
    return m_pluginWidget.data();
}

QWidget *PrivacyIndicatorPlugin::itemTipsWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    if (!m_tipsWidget) {
        m_tipsWidget = new QLabel();
        updateTooltip();
    }

    return m_tipsWidget;
}

void PrivacyIndicatorPlugin::init(PluginProxyInterface *proxyInter)
{
    qDebug() << "[PrivacyIndicator] init() called";
    m_proxyInter = proxyInter;

    if (!pluginIsDisable()) {
        qDebug() << "[PrivacyIndicator] Plugin is enabled, loading...";
        loadPlugin();
    } else {
        qDebug() << "[PrivacyIndicator] Plugin is disabled, skipping load";
    }
}

void PrivacyIndicatorPlugin::pluginStateSwitched()
{
    qDebug() << "[PrivacyIndicator] pluginStateSwitched() called";
    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, !m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool());

    if (pluginIsDisable()) {
        qDebug() << "[PrivacyIndicator] Disabling plugin";
        m_proxyInter->itemRemoved(this, pluginName());
    } else {
        qDebug() << "[PrivacyIndicator] Enabling plugin";
        if (!m_pluginLoaded) {
            loadPlugin();
            return;
        }
        m_proxyInter->itemAdded(this, pluginName());
    }
}

bool PrivacyIndicatorPlugin::pluginIsAllowDisable()
{
    return true;
}

bool PrivacyIndicatorPlugin::pluginIsDisable()
{
    bool disabled = !m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool();
    qDebug() << "[PrivacyIndicator] pluginIsDisable() returns:" << disabled;
    return disabled;
}

int PrivacyIndicatorPlugin::itemSortKey(const QString &itemKey)
{
    const QString key = QString("pos_%1_%2").arg(itemKey).arg(Dock::Efficient);
    return m_proxyInter->getValue(this, key, 100).toInt();
}

void PrivacyIndicatorPlugin::setSortKey(const QString &itemKey, const int order)
{
    const QString key = QString("pos_%1_%2").arg(itemKey).arg(Dock::Efficient);
    m_proxyInter->saveValue(this, key, order);
}

Dock::PluginFlags PrivacyIndicatorPlugin::flags() const
{
    qDebug() << "[PrivacyIndicator] flags() called";
    return Dock::Type_System | Dock::Attribute_Normal;
}

void PrivacyIndicatorPlugin::loadPlugin()
{
    qDebug() << "[PrivacyIndicator] loadPlugin() called";
    
    if (m_pluginLoaded) {
        qDebug() << "[PrivacyIndicator] Plugin already loaded, skipping";
        return;
    }

    m_pluginLoaded = true;
    qDebug() << "[PrivacyIndicator] Creating PrivacyWidget and DeviceMonitor...";

    m_pluginWidget.reset(new PrivacyWidget());
    m_deviceMonitor = new DeviceMonitor(this);

    connect(m_deviceMonitor, &DeviceMonitor::deviceStateChanged,
            this, &PrivacyIndicatorPlugin::onDeviceStateChanged);

    // 将设备状态转发到控件
    connect(m_deviceMonitor, &DeviceMonitor::deviceStateChanged,
            m_pluginWidget.data(), &PrivacyWidget::setDeviceStatus);

    qDebug() << "[PrivacyIndicator] Adding plugin item to dock...";
    m_proxyInter->itemAdded(this, pluginName());
    qDebug() << "[PrivacyIndicator] Plugin loaded successfully!";
}

void PrivacyIndicatorPlugin::updateTooltip()
{
    if (!m_tipsWidget || !m_deviceMonitor) {
        return;
    }

    QStringList activeDevices;
    if (m_deviceMonitor->isCameraInUse()) {
        activeDevices << QObject::tr("摄像头");
    }
    if (m_deviceMonitor->isMicrophoneInUse()) {
        activeDevices << QObject::tr("麦克风");
    }
    if (m_deviceMonitor->isLocationInUse()) {
        activeDevices << QObject::tr("位置");
    }

    if (activeDevices.isEmpty()) {
        m_tipsWidget->setText(QObject::tr("隐私指示器"));
    } else {
        m_tipsWidget->setText(QObject::tr("隐私指示器\n使用中: %1").arg(activeDevices.join(", ")));
    }
}

void PrivacyIndicatorPlugin::onDeviceStateChanged(DeviceType type, bool inUse)
{
    Q_UNUSED(type);
    Q_UNUSED(inUse);

    updateTooltip();

    if (m_pluginWidget) {
        m_pluginWidget->update();
    }
}
