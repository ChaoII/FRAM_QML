import QtQuick
import QtQuick.VirtualKeyboard
import HuskarUI.Basic
import MyApp 1.0

HusWindow {
    id: window
    property var detectionResults: []
    property int currentFrameSequence: 0
    property var frameTimestamps: ({})  // 存储帧时间戳的字典
    property int lastDetectionSequence: -1  // 上次显示的检测结果序列号
    
    captionBar.visible: false
    width: 640
    height: 480
    title: qsTr("FRAM")
    visible: true



    function descaleFaceResult(x, y, width, height, fillMode) {
        var x_ = x;
        var y_ = y;
        var width_ = width;
        var height_ = height;

        // 获取原始图像尺寸和显示尺寸
        var sourceWidth = processedImage.sourceSize.width;
        var sourceHeight = processedImage.sourceSize.height;
        var displayWidth = processedImage.width;
        var displayHeight = processedImage.height;

        // 防止除零错误
        if (sourceWidth <= 0 || sourceHeight <= 0) {
            return {
                "x": x_,
                "y": y_,
                "width": width_,
                "height": height_
            };
        }

        // 计算基础缩放比例
        var scaleX = displayWidth / sourceWidth;
        var scaleY = displayHeight / sourceHeight;
        var scale, scaledSourceWidth, scaledSourceHeight, offsetX, offsetY;

        if (fillMode === Image.Stretch) {
            // Stretch: 完全拉伸填满，宽高比可能改变
            x_ = x * scaleX;
            y_ = y * scaleY;
            width_ = width * scaleX;
            height_ = height * scaleY;
        } else if (fillMode === Image.PreserveAspectFit) {
            // PreserveAspectFit: 保持宽高比，完整显示图像，可能有黑边
            scale = Math.min(scaleX, scaleY);
            scaledSourceWidth = sourceWidth * scale;
            scaledSourceHeight = sourceHeight * scale;
            offsetX = (displayWidth - scaledSourceWidth) / 2;
            offsetY = (displayHeight - scaledSourceHeight) / 2;

            x_ = x * scale + offsetX;
            y_ = y * scale + offsetY;
            width_ = width * scale;
            height_ = height * scale;
        } else if (fillMode === Image.PreserveAspectCrop) {
            // PreserveAspectCrop: 保持宽高比，填满显示区域，可能裁剪
            scale = Math.max(scaleX, scaleY);
            scaledSourceWidth = sourceWidth * scale;
            scaledSourceHeight = sourceHeight * scale;
            offsetX = (displayWidth - scaledSourceWidth) / 2;
            offsetY = (displayHeight - scaledSourceHeight) / 2;

            x_ = x * scale + offsetX;
            y_ = y * scale + offsetY;
            width_ = width * scale;
            height_ = height * scale;
        } else if (fillMode === Image.Tile || fillMode === Image.Pad)
        // 不做任何缩放处理
        {}
        // 默认情况 (Image.Null): 不缩放

        return {
            "x": x_,
            "y": y_,
            "width": width_,
            "height": height_
        };
    }



    Connections {
        function onCameraFrameUpdated(camera_id) {
            // 强制刷新，避免 Image 缓存
            processedImage.updateImage(camera_id);
            // 增加帧序列号
            currentFrameSequence++;
        }
        
        function onFaceResultUpdate(camera_id, results, timestamp, sequence) {
            // 直接使用最新的检测结果，不做预测或插值
            detectionResults = results;
            lastDetectionSequence = sequence;
        }

        target: cameraManager
    }
    
    Image {
        id: processedImage

        property real offsetX: (processedImage.width - processedImage.sourceSize.width * ratio) / 2
        property real offsetY: (processedImage.height - processedImage.sourceSize.height * ratio) / 2
        property real ratio: Math.min(processedImage.width / processedImage.sourceSize.width, processedImage.height / processedImage.sourceSize.height)

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
                // console.log(selector.model);
                cameraManager.createCamera(selector.currentIndex, VideoProcessor.Local);
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
    
    Repeater {
        model: detectionResults   // 直接使用检测结果

        delegate: Item {
            property var rect: descaleFaceResult(modelData.x, modelData.y, modelData.width, modelData.height, processedImage.fillMode)

            height: rect.height
            width: rect.width
            x: rect.x
            y: rect.y

            Rectangle {
                anchors.fill: parent
                border.color: "red"
                border.width: 2
                color: "transparent"
            }
            Text {
                anchors.bottom: parent.top
                anchors.left: parent.left
                color: "yellow"
                font.pixelSize: 14
                text: "ID:" + modelData.label_id + " " + (modelData.score * 100).toFixed(1) + "%"
            }
        }
    }
}