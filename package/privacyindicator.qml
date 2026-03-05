// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D
import org.deepin.ds.dock 1.0

AppletItem {
    id: privacyIndicator

    property bool useColumnLayout: Panel.position % 2
    property int dockOrder: 10
    
    // 调试模式：设为 true 可强制显示插件（红色背景）
    property bool debugMode: false
    
    // 图标大小
    property int iconSize: Math.min(Panel.rootObject.dockItemMaxSize * 0.6, 32)
    
    // 计算激活的图标数量
    property int activeIconCount: (Applet.cameraInUse ? 1 : 0) + (Applet.microphoneInUse ? 1 : 0) + (Applet.locationInUse ? 1 : 0)

    implicitWidth: useColumnLayout ? Panel.rootObject.dockSize : (debugMode ? iconSize : (Applet.anyDeviceInUse ? activeIconCount * iconSize : 0))
    implicitHeight: useColumnLayout ? (debugMode ? iconSize : (Applet.anyDeviceInUse ? activeIconCount * iconSize : 0)) : Panel.rootObject.dockSize

    visible: debugMode || Applet.anyDeviceInUse
    
    // 调试背景色
    Rectangle {
        anchors.fill: parent
        color: debugMode ? "red" : "transparent"
        visible: debugMode || Applet.anyDeviceInUse
        opacity: debugMode ? 0.5 : 0
    }

    // 提示框
    PanelToolTip {
        id: toolTip
        text: Applet.getTooltipText() || ""
        toolTipX: DockPanelPositioner.x
        toolTipY: DockPanelPositioner.y
    }

    // 水平布局
    Row {
        id: horizontalLayout
        anchors.centerIn: parent
        spacing: 4
        visible: !useColumnLayout && (Applet.anyDeviceInUse || debugMode)

        // 摄像头图标
        Image {
            visible: Applet.cameraInUse || debugMode
            source: "icons/camera-video.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }

        // 麦克风图标
        Image {
            visible: Applet.microphoneInUse || debugMode
            source: "icons/audio-input-microphone.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }

        // 位置图标
        Image {
            visible: Applet.locationInUse || debugMode
            source: "icons/gps.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }
    }

    // 垂直布局
    Column {
        id: verticalLayout
        anchors.centerIn: parent
        spacing: 4
        visible: useColumnLayout && (Applet.anyDeviceInUse || debugMode)

        // 摄像头图标
        Image {
            visible: Applet.cameraInUse || debugMode
            source: "icons/camera-video.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }

        // 麦克风图标
        Image {
            visible: Applet.microphoneInUse || debugMode
            source: "icons/audio-input-microphone.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }

        // 位置图标
        Image {
            visible: Applet.locationInUse || debugMode
            source: "icons/gps.svg"
            width: iconSize
            height: iconSize
            sourceSize: Qt.size(iconSize, iconSize)
        }
    }

    // 鼠标区域
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            if (Applet.anyDeviceInUse || debugMode) {
                var point = privacyIndicator.mapToItem(null, privacyIndicator.width / 2, privacyIndicator.height / 2)
                toolTip.DockPanelPositioner.bounding = Qt.rect(point.x, point.y, toolTip.width, toolTip.height)
                toolTip.open()
            }
        }

        onExited: {
            toolTip.close()
        }
    }

    Component.onCompleted: {
        console.log("Privacy indicator plugin loaded, debugMode:", debugMode)
        console.log("Camera:", Applet.cameraInUse, "Microphone:", Applet.microphoneInUse, "Location:", Applet.locationInUse)
    }
}
