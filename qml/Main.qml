import QtQuick
import QtQuick.VirtualKeyboard
import HuskarUI.Basic
import MyApp 1.0

HusWindow {
    id: window

    captionBar.visible: false
    height: 480
    title: qsTr("Hello World")
    visible: true
    width: 640

    Connections {
        function onCameraFrameUpdated(camera_id) {
            // 强制刷新，避免 Image 缓存
            processedImage.updateImage(camera_id);
        }

        target: cameraManager
    }
    Image {
        id: processedImage

        function updateImage(camera_id) {
            source = "image://" + camera_id + "/current_" + Date.now();
        }

        anchors.fill: parent
        cache: false
        fillMode: Image.PreserveAspectFit

        HusSelect {
            id: selector

            model: localCamera.cameras
            textRole: "description"
            width: 160
            x: 20
            y: 20

            onCurrentIndexChanged: index => {
                                       let selected = selector.model[selector.currentIndex];
                                       console.log(selected.description);
                                   }
        }
        HusButton {
            id: buttonStartCamera

            anchors.left: selector.right
            anchors.leftMargin: 5
            anchors.verticalCenter: selector.verticalCenter
            text: "打开摄像头"
            type: HusButton.Type_Primary

            onClicked: {
                console.log(selector.model);

                cameraManager.createCamera(selector.currentIndex, CameraDecoder.Local);
                cameraManager.startCamera(selector.currentIndex);
            }
        }
        HusButton {
            id: buttonStopCamera

            anchors.left: buttonStartCamera.right
            anchors.leftMargin: 5
            anchors.verticalCenter: buttonStartCamera.verticalCenter
            text: "关闭摄像头"
            type: HusButton.Type_Primary

            onClicked: {
                cameraManager.stopCamera(selector.currentIndex);
            }
        }
        // HusButton {
        //     id: button2

        //     text: "开启检测"
        //     type: HusButton.Type_Primary
        //     x: 20
        //     y: 100

        //     onClicked: {
        //         detector.startDetection();
        //         attendInfo.visible = true;
        //     }
        // }
        AttendInfo {
            id: attendInfo

            height: 160
            width: parent.width
            x: 0
            y: parent.height - 200
            z: 30
        }
    }
}
