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
                    var filePath = "C:/Users/84945/Pictures/capture_" + Date.now() + ".png"
                    // 最多3个
                    if (faceListModel.count >= 3) {
                        faceListModel.remove(0)
                    }
                    result.saveToFile(filePath)
                    faceListModel.append({"url":"file:///"+filePath})
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
                    if (path.startsWith("file:///")) {
                        path = path.substring(8)// 去掉 file:///
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
        if(CameraManager.isCameraActivated){
            CameraManager.stopCamera()
        }
        CameraManager.videoOutput = registerPreview
        console.log("RegisterPage created, starting camera...")
        CameraManager.startCamera()
        addBtn.forceActiveFocus()
    }

    // Component.onDestruction: {
    //     console.log("RegisterPage destroyed, stopping camera...")
    //     registerCamera.stop()
    // }
}
