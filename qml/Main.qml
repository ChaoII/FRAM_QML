import QtQuick
import QtQuick.Controls
import QtMultimedia
import MyApp
import HuskarUI.Basic
// 导入模块的同时将模块中的单例类进行实例化
import fram_qml
import AttendInfo2


HusWindow {
    id: root
    width: 480
    height: 600
    visible: true
    title: ""
    captionBar.visible: false
    AttendInfo2{}
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {
            stackView: stackView
        }  // 把stackView传进去
    }
}
