import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import MyApp
import HuskarUI.Basic


Page {
    id: registerPage
    property var stackView: null

    RegisterFace{
        id:registerFace
        onRegisterReady:function(){
            message.info("注册成功")
            faceListModel.clear()
        }
        onRegisterError:function(){
            message.error("注册失败, 请确保画面中包含人脸")
            faceListModel.clear()
        }
    }


    VideoOutput {
        id: registerPreview
        anchors.fill:parent
        fillMode: VideoOutput.PreserveAspectCrop
    }
    Item{
        anchors.fill:parent
        Canvas {
            id: faceMask
            anchors.fill: parent
            z: 1

            onPaint: {
                var ctx = getContext("2d")
                ctx.reset()

                // 绘制半透明黑色背景
                ctx.fillStyle = "#80000000"
                ctx.fillRect(0, 0, width, height)

                // 设置不绘制模式（挖空人脸区域）
                ctx.globalCompositeOperation = "destination-out"

                // 绘制人脸轮廓（包括耳朵和肩膀）
                var centerX = width / 2
                var centerY = height / 2
                var faceWidth = Math.min(width, height) * 0.6
                var faceHeight = faceWidth * 1.3

                ctx.beginPath()

                // 头顶部分（半圆）
                ctx.arc(centerX, centerY - faceHeight * 0.35, faceWidth * 0.5, Math.PI, 0, false)

                // 右耳
                ctx.lineTo(centerX + faceWidth * 0.55, centerY - faceHeight * 0.2)
                ctx.quadraticCurveTo(
                    centerX + faceWidth * 0.6, centerY - faceHeight * 0.1,
                    centerX + faceWidth * 0.5, centerY + faceHeight * 0.1
                )

                // 下巴到右肩
                ctx.lineTo(centerX + faceWidth * 0.4, centerY + faceHeight * 0.5)
                ctx.quadraticCurveTo(
                    centerX + faceWidth * 0.25, centerY + faceHeight * 0.6,
                    centerX, centerY + faceHeight * 0.55
                )

                // 左肩到左耳
                ctx.quadraticCurveTo(
                    centerX - faceWidth * 0.25, centerY + faceHeight * 0.6,
                    centerX - faceWidth * 0.4, centerY + faceHeight * 0.5
                )
                ctx.lineTo(centerX - faceWidth * 0.5, centerY + faceHeight * 0.1)
                ctx.quadraticCurveTo(
                    centerX - faceWidth * 0.6, centerY - faceHeight * 0.1,
                    centerX - faceWidth * 0.55, centerY - faceHeight * 0.2
                )

                ctx.closePath()
                ctx.fill()

                // 恢复绘制模式
                ctx.globalCompositeOperation = "source-over"

                // 绘制白色边框
                ctx.strokeStyle = "white"
                ctx.lineWidth = 2
                ctx.beginPath()

                // 头顶部分（半圆）
                ctx.arc(centerX, centerY - faceHeight * 0.35, faceWidth * 0.5, Math.PI, 0, false)

                // 右耳
                ctx.lineTo(centerX + faceWidth * 0.55, centerY - faceHeight * 0.2)
                ctx.quadraticCurveTo(
                    centerX + faceWidth * 0.6, centerY - faceHeight * 0.1,
                    centerX + faceWidth * 0.5, centerY + faceHeight * 0.1
                )

                // 下巴到右肩
                ctx.lineTo(centerX + faceWidth * 0.4, centerY + faceHeight * 0.5)
                ctx.quadraticCurveTo(
                    centerX + faceWidth * 0.25, centerY + faceHeight * 0.6,
                    centerX, centerY + faceHeight * 0.55
                )

                // 左肩到左耳
                ctx.quadraticCurveTo(
                    centerX - faceWidth * 0.25, centerY + faceHeight * 0.6,
                    centerX - faceWidth * 0.4, centerY + faceHeight * 0.5
                )
                ctx.lineTo(centerX - faceWidth * 0.5, centerY + faceHeight * 0.1)
                ctx.quadraticCurveTo(
                    centerX - faceWidth * 0.6, centerY - faceHeight * 0.1,
                    centerX - faceWidth * 0.55, centerY - faceHeight * 0.2
                )

                ctx.stroke()

                // 绘制四个角标
                var cornerSize = 30
                var cornerLength = 15

                function drawCorner(x, y, rotation) {
                    ctx.save()
                    ctx.translate(x, y)
                    ctx.rotate(rotation * Math.PI / 180)

                    ctx.beginPath()
                    ctx.moveTo(0, cornerLength)
                    ctx.lineTo(0, 0)
                    ctx.lineTo(cornerLength, 0)
                    ctx.stroke()

                    ctx.restore()
                }

                // 左上角
                drawCorner(centerX - faceWidth * 0.55, centerY - faceHeight * 0.35, 0)
                // 右上角
                drawCorner(centerX + faceWidth * 0.55, centerY - faceHeight * 0.35, 90)
                // 右下角
                drawCorner(centerX + faceWidth * 0.55, centerY + faceHeight * 0.55, 180)
                // 左下角
                drawCorner(centerX - faceWidth * 0.55, centerY + faceHeight * 0.55, 270)
            }
        }

        // 提示文字
        Text {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            text: "请将人脸对准轮廓区域"
            color: "white"
            font.pixelSize: 20
        }
    }


    Column{
        x:20
        y:20
        spacing: 10
        RowLayout{
            HusText{
                text:"姓名:"
            }
            HusInput {
                id: nameInput
                colorBg: "#00000000"
                implicitWidth: 120
                placeholderText: "请输入姓名"
            }
        }

        RowLayout{
            Layout.alignment: Qt.AlignVCenter
            HusText{
                text:"工号:"
            }
            HusInput {
                id: staffNoInput
                colorBg: "#00000000"
                implicitWidth: 120
                placeholderText: "请输入工号"
            }
        }
    }

    RowLayout {
        id: row
        spacing: 20
        anchors.bottom: cl.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter  // 整个 RowLayout 居中
        Repeater {
            model: faceListModel
            delegate: Item {
                width: registerPage.width/5
                height: registerPage.height/5
                Image {
                    anchors.fill: parent
                    source: model.url   // Repeater 内要用 modelData
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        onClicked: faceListModel.remove(index)
                    }
                }
            }
        }
    }

    ListModel { id: faceListModel }
    HusMessage {
        id: message
        z: 999
        parent: root.captionBar
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
    }

    RowLayout{
        id:cl
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        Layout.alignment: Qt.AlignVCenter
        spacing: 10
        HusButton {
            text: "返回"
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                }
            }
        }
        HusButton {
            id:addBtn
            text: "添加"
            focus: true
            onClicked: {
                registerPreview.grabToImage(function (result) {
                    var filePath = "C:/Users/aichao/Pictures/capture_" + Date.now() + ".png"
                    // 最多3个
                    if (faceListModel.count >= 3) {
                        faceListModel.remove(0)
                    }
                    result.saveToFile(filePath)
                    faceListModel.append({"url":"file:"+filePath})
                })
            }
        }
        HusButton {
            text: "注册"
            onClicked: {
                var name = nameInput.text
                var staffNo = staffNoInput.text
                if(!name||!staffNo){
                    message.error("姓名或工号不能为空")
                    return
                }
                if(faceListModel.count===0){
                    message.error("人脸列表不能为空，请添加人脸")
                    return
                }
                var paths = []
                for (var i = 0; i < faceListModel.count; i++) {
                    // 去掉 file:/// 前缀
                    var path = faceListModel.get(i).url;
                    if (path.startsWith("file:")) {
                        path = path.substring(5)// 去掉 file:///
                    }
                    paths.push(path)
                }
                registerFace.processImages(name,staffNo,paths)
            }
        }
    }

    Connections {

    }

    Component.onCompleted: {
        Qt.callLater(function(){
            CameraManager.videoOutput = registerPreview
            console.log("RegisterPage created, starting camera...")
        })
    }

    // Component.onDestruction: {
    //     console.log("RegisterPage destroyed, stopping camera...")
    //     registerCamera.stop()
    // }
}
