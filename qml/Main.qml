import QtQuick
import QtQuick.Controls
import QtMultimedia
import MyApp
import HuskarUI.Basic

HusWindow {
    id: root
    width: 480
    height: 600
    visible: true
    title: ""
    property alias cameraManager: cameraManager

    CameraManager {
        id: cameraManager
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage { stackView: stackView }  // 把stackView传进去
    }
}
