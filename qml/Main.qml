import QtQuick
import QtQuick.Controls
import QtMultimedia
import MyApp
import HuskarUI.Basic

HusWindow {
    id: root
    captionBar.visible: !ConfigManager.headlessMode
    height: 720
    title: ""
    visible: true
    width: 480

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {
            stackView: stackView
        }  // 把stackView传进去
    }
    Component.onCompleted: HusTheme.darkMode = ConfigManager.themeMode
}
