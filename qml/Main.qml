import QtQuick
import QtQuick.Controls
import QtMultimedia
import MyApp
import HuskarUI.Basic

HusWindow {
    id: root
    width: 480
    height: 720
    maximumHeight: 720
    minimumHeight: 720
    maximumWidth: 480
    minimumWidth: 480
    title: ""
    visible: true
    captionBar.visible: !ConfigManager.headlessMode

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {
            stackView: stackView
        }
    }
    Component.onCompleted: HusTheme.darkMode = ConfigManager.themeMode
}
