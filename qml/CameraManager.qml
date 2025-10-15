// CameraManager.qml
import QtQuick
import QtMultimedia

QtObject {
    id: cameraManager
    // 摄像头属性
    property Camera camera: Camera
    {
        id: camera
        active: true
    }

    property ImageCapture imageCapture: ImageCapture
    {
        id: imageCapture
    }

    property alias isCameraActivated: camera.active
    property MediaDevices mediaDevices: MediaDevices
    {
    }
    // 视频输出（需要在界面中设置具体实例）
    property VideoOutput videoOutput

    property CaptureSession captureSession: CaptureSession
    {
        id: captureSession
        camera: camera
        imageCapture: imageCapture
    }

    // 可用摄像头列表
    property var availableCameras: mediaDevices.videoInputs

    // 当前选中的摄像头设备
    property var currentDevice

    // 初始化摄像头
    function initializeCamera(deviceId) {
        if (availableCameras.length === 0) {
            console.warn("No cameras available")
            return
        }

        // 查找设备
        const device = availableCameras.find(cam => cam.id.toString() === deviceId.toString()) || availableCameras[0]
        currentDevice = device
        camera.cameraDevice = device

        // // 默认配置
        // camera.resolution = "1280x720"
        // camera.frameRate = 30
    }

    // 切换摄像头
    function switchCamera(deviceId) {
        initializeCamera(deviceId)
    }

    // 开始摄像头
    function startCamera() {
        camera.start()
    }

    // 停止摄像头
    function stopCamera() {
        camera.stop()
    }

    onVideoOutputChanged: {
        captureSession.videoOutput = videoOutput
    }

    Component.onCompleted: {
        if (availableCameras.length > 0) {
            initializeCamera(mediaDevices.defaultVideoInput)
        }
    }
}
