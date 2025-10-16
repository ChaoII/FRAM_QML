import QtQuick
import QtQuick.Controls
import QtMultimedia
import HuskarUI.Basic
import MyApp


Item {
    id: mainPage
    property var stackView: null

    Timer {
        id: hideTimer
        interval: 2000
        repeat: false
        onTriggered: attendInfo.visible = false
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
        FrameAnalyzer {
            id: processor
            videoSink: videoOutput.videoSink
            dstSize: Qt.size(mainPage.width, mainPage.height)
            onDetectionRect: function (rects) {
                detRect.model = rects
            }

            onRecognitionResult: function (recRet) {
                attendInfo.visible = true
                attendInfo.isUnknown = recRet.isUnknown
                attendInfo.staffNo = recRet.staffNo
                attendInfo.name = recRet.name
                attendInfo.baseImageSource = "file:" + recRet.picUrl
                attendInfo.attendTime = recRet.attendTime
                videoOutput.grabToImage(function (result) {
                    attendInfo.curImageSource = result.url
                })
                hideTimer.restart()
            }
        }
    }

    HusText {
        id: statusText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 8
        text: "摄像头状态: " + (CameraManager.isCameraActive ? "运行中" : "已停止")
        color: "white"
    }

    Repeater {
        id: detRect
        model: []
        delegate: Item {
            Rectangle {
                x: modelData.x; y: modelData.y
                width: modelData.width; height: modelData.height
                border.color: "yellow"; border.width: 2
                color: "transparent"
            }
            Repeater {
                model: modelData.landmarks
                Rectangle {
                    x: modelData.x - parent.x
                    y: modelData.y - parent.y
                    width: 8
                    height: 8
                    radius: 4
                    color: "#ff00ff"
                }
            }
        }
    }

    Component {
        id: registerComponent
        RegisterPage {
            stackView: mainPage.stackView
        }
    }
    Component {
        id: settingsComponent
        Settings {
            stackView: mainPage.stackView
        }
    }

    Component {
        id: attendhistoryComponent
        AttendHistory {
            stackView: mainPage.stackView
        }
    }


    Row {
        id: controls
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        HusButton {
            id: btnAttendHistory
            text: "打卡记录"
            onClicked: {
                stackView.push(attendhistoryComponent)
                if (CameraManager.isCameraActivated) {
                    CameraManager.stopCamera()
                }
            }
        }

        HusButton {
            id: btnRegister
            text: "注册"
            onClicked: {
                stackView.push(registerComponent)
            }
        }

        HusButton {
            id: btnSettings
            text: "设置"
            onClicked: {
                stackView.push(settingsComponent)
            }
        }
    }

    AttendInfo {
        id: attendInfo
        visible: false
        height: 160
        width: parent.width
        y: parent.height - 200
        z: 9999
        opacity: 0.9
        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.4
        }
    }

    Component.onCompleted: {
        CameraManager.videoOutput = videoOutput
        CameraManager.startCamera()
    }

    Connections {
        target: stackView
        function onCurrentItemChanged() {
            if (stackView.currentItem === mainPage) {
                if (CameraManager.isCameraActivated) {
                    CameraManager.stopCamera()
                }
                CameraManager.videoOutput = videoOutput
                CameraManager.startCamera()
            }
        }
    }
}
