import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtMultimedia
import MyApp
import HuskarUI.Basic
import Fram

Page {
    id: registerPage
    property var stackView: null

    RegisterFace{
        id:registerFace
        onRegisterFinished:function(){
            message.info("注册成功")
            faceListModel.clear()
            if (stackView.depth > 1) {
                stackView.pop()
            }
        }
        onRegisterFailed:function(errorMsg){
            message.error("注册失败, 请确保画面中包含人脸:"+errorMsg)
            faceListModel.clear()
        }
    }

    QmlImageUtils{
        id:qmlImageUtils
    }


    VideoOutput {
        id: registerPreview
        anchors.fill:parent
        fillMode: VideoOutput.PreserveAspectCrop
    }

    //人脸遮罩
    Canvas {
        id:mask
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            // 半透明遮罩
            ctx.fillStyle = "rgba(0, 0, 0, 0.6)";
            ctx.fillRect(0, 0, width, height);

            // 挖出中间的椭圆形
            var rx = 300;
            var ry = 400;
            var cx = (width-rx) / 2
            // 居中往上移60像素
            var cy = (height-ry) / 2 - 60

            ctx.globalCompositeOperation = "destination-out";
            ctx.beginPath();
            ctx.ellipse(cx, cy, rx, ry, 0, 0, 2 * Math.PI);
            ctx.fill();
            ctx.globalCompositeOperation = "source-over";

            // 画个白色边框
            ctx.strokeStyle = "white";
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.ellipse(cx, cy, rx, ry, 0, 0, 2 * Math.PI);
            ctx.stroke();
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
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 10
        z:999
    }

    RowLayout{
        id:cl
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        Layout.alignment: Qt.AlignVCenter
        spacing: 10

        HusButton {
            text: "弹走鱼尾纹"
            onClicked: {
                message.error("asdasdasdasdasd")
            }
        }

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
                // 该方法是异步的需要等回调
                CameraManager.captureImage()
            }
        }
        HusButton {
            text: "注册"
            onClicked: {
                if(faceListModel.count===0){
                    message.error("人脸列表不能为空，请添加人脸")
                    return
                }
                var paths = []
                for (var i = 0; i < faceListModel.count; i++) {
                    var path = faceListModel.get(i).url;
                    if (path.startsWith("file:")) {
                        path = path.substring(5)// 去掉 file:
                    }
                    paths.push(path)
                }
                registerFace.processImages(nameInput.text,staffNoInput.text,paths)
            }
        }
    }

    Connections {
        target: CameraManager
        function onImageCaptureSuccess(id,image){
            // 最多3个
            var name = nameInput.text
            var staffNo = staffNoInput.text
            if(!name||!staffNo){
                message.error("姓名或工号不能为空")
                return
            }
            var tempFilePath = qmlImageUtils.getTempPicUrl()
            let cropImage = qmlImageUtils.cropImage(
                    image, Qt.size(registerPage.width,registerPage.height)
                    )
            if(!qmlImageUtils.saveImage(cropImage,tempFilePath)){
                message.error("图片保存失败，请稍后再试")
            }
            if (faceListModel.count >= 3) {
                // 删除第一个文件
                let path = faceListModel.get(0).url
                if (path.startsWith("file:")) {
                    path = path.substring(5)// 去掉 file:///
                }
                if (qmlImageUtils.removeFile(path)) {
                    faceListModel.remove(0)
                    console.log("文件删除成功: ", path);
                } else {
                    console.log("文件删除失败:", file.errorString());
                }
            }
            faceListModel.append({"url":"file:" + tempFilePath})
            console.log("添加成功: ", tempFilePath);
        }
        function onImageCaptureFailed(id,error,msg){
            console.error("文件保存失败:", error, msg);
        }
    }

    Component.onCompleted: {
        Qt.callLater(function(){
            CameraManager.videoOutput = registerPreview
        })
        console.log(stackView.depth)
    }
}
