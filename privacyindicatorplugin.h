#ifndef PRIVACYINDICATORPLUGIN_H
#define PRIVACYINDICATORPLUGIN_H

#include <dde-dock/pluginsiteminterface_v2.h>
#include "privacywidget.h"
#include "devicemonitor.h"
#include <QLabel>
#include <QScopedPointer>

class PrivacyIndicatorPlugin : public QObject, public PluginsItemInterfaceV2
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface_V2" FILE "privacy-indicator.json")

public:
    explicit PrivacyIndicatorPlugin(QObject *parent = nullptr);

    const QString pluginName() const override;
    const QString pluginDisplayName() const override;
    void init(PluginProxyInterface *proxyInter) override;

    void pluginStateSwitched() override;
    bool pluginIsAllowDisable() override;
    bool pluginIsDisable() override;

    QWidget *itemWidget(const QString &itemKey) override;
    QWidget *itemTipsWidget(const QString &itemKey) override;

    Dock::PluginFlags flags() const override;

    int itemSortKey(const QString &itemKey) override;
    void setSortKey(const QString &itemKey, const int order) override;

private:
    void loadPlugin();
    void updateTooltip();

private slots:
    void onDeviceStateChanged(DeviceType type, bool inUse);

private:
    bool m_pluginLoaded;
    QScopedPointer<PrivacyWidget> m_pluginWidget;
    QLabel *m_tipsWidget;
    DeviceMonitor *m_deviceMonitor;
};

#endif // PRIVACYINDICATORPLUGIN_H
