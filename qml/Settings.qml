import QtQuick
import QtMultimedia
import HuskarUI.Basic

Item {
    property var stackView: null
    property alias cameraManager: cameraManager
    CameraManager {
        id: cameraManager
    }
    HusSelect {
        x: 20
        y: 80
        id: cameraSelector
        model: cameraManager.availableCameras
        textRole: "description"
        valueRole: []
        onActivated: {
            cameraManager.switchCamera(currentValue.id)
        }
        z: 100
    }

    HusText {
        x: 200
        y: 80
        text: "Camera status: " + (cameraManager.camera.active ? "Active" : "Inactive")
        color: "black"
    }

    VideoOutput {
        id: videoOutput
        x: 20
        y: 120
        width: 120
        height: 160
        fillMode: VideoOutput.PreserveAspectCrop
        // 在组件完成时设置 videoOutput
        Component.onCompleted: {
            cameraManager.videoOutput = videoOutput
        }
    }

    HusText {
        id: t_threshold
        x: 20
        y: 40
        text: "识别阈值："
    }

    HusInputNumber {
        id: threshold
        width: 120
        anchors.verticalCenter: t_threshold.verticalCenter
        anchors.left: t_threshold.right
        anchors.leftMargin: 8
        value: 0.62
        step: 0.01
        precision: 2
    }

    HusButton {
        text: "返回"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        onClicked: {
            if (stackView.depth > 1) {
                stackView.pop()
            }
        }
    }

    HusButton {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 100
        anchors.bottomMargin: 20
        text: "Apply Settings"
        onClicked: {
            // 应用其他设置...
            cameraManager.startCamera()
        }
    }
}
