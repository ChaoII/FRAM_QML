import QtQuick
import QtMultimedia
import QtQuick.Layouts
import HuskarUI.Basic
import MyApp

Item {
    property var stackView: null

    function getIndexFromAfterLabel(label) {
        switch (label) {
        case "日":
            return 0;
        case "月":
            return 1;
        case "年":
            return 2;
        }
    }
    function saveConfig() {
        {
            ConfigManager.headlessMode = headlessMode.checked;
            ConfigManager.detThreshold = detThreshold.value;
            ConfigManager.recThreshold = recThreshold.value;
            ConfigManager.faceDBNum = faceDBNum.value;
            ConfigManager.clearOldUnit = getIndexFromAfterLabel(clearOldSince.currentAfterLabel);
            ConfigManager.clearOldInterval = clearOldSince.value;
            ConfigManager.attendCacheInterval = attendCacheInterval.value;
            ConfigManager.themeMode = themeMode.currentIndex;
            ConfigManager.saveConfig();
            modal.openWarning()
        }
    }


    HusModal {
             id: modal
             width: 300
             title: '消息提示'
             description: '修改成功是否重启'
             confirmText: '确认'
             cancelText: '取消'
             onConfirm: ConfigManager.restart();
             onCancel: close();
         }


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "选择摄像头："
            }
            HusSelect {
                id: cameraSelector

                Layout.fillWidth: true
                model: CameraManager.availableCameras
                textRole: "description"
                valueRole: []
                z: 100

                onActivated: {
                    CameraManager.switchCamera(currentValue.id);
                }
            }
        }
        VideoOutput {
            id: videoOutput

            Layout.preferredHeight: 150
            Layout.preferredWidth: 120
            fillMode: VideoOutput.PreserveAspectCrop

            // 在组件完成时设置 videoOutput
            Component.onCompleted: {
                CameraManager.videoOutput = videoOutput;
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "无头模式："
            }
            HusSwitch {
                id: headlessMode
                Layout.fillWidth: true
                checked: ConfigManager.headlessMode
                checkedText: qsTr('开启')
                effectEnabled: false
                uncheckedText: qsTr('关闭')
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "检测阈值："
            }
            HusInputNumber {
                id: detThreshold

                Layout.fillWidth: true
                precision: 1
                step: 0.1
                value: ConfigManager.detThreshold
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "识别阈值："
            }
            HusInputNumber {
                id: recThreshold

                Layout.fillWidth: true
                precision: 2
                step: 0.01
                value: ConfigManager.recThreshold
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "单人脸限制："
            }
            HusInputNumber {
                id: faceDBNum

                Layout.fillWidth: true
                value: ConfigManager.faceDBNum
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "清除打卡记录："
            }
            HusInputNumber {
                id: clearOldSince

                Layout.fillWidth: true
                afterLabel: [
                    {
                        label: '日',
                        value: 0
                    },
                    {
                        label: '月',
                        value: 1
                    },
                    {
                        label: '年',
                        value: 2
                    }
                ]
                animationEnabled: false
                initAfterLabelIndex: ConfigManager.clearOldUnit
                value: ConfigManager.clearOldInterval
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "打卡记录缓存："
            }
            HusInputNumber {
                id: attendCacheInterval

                Layout.fillWidth: true
                afterLabel: [
                    {
                        label: '秒',
                        value: 0
                    }
                ]
                animationEnabled: false
                value: ConfigManager.attendCacheInterval
            }
        }
        RowLayout {
            HusText {
                Layout.preferredWidth: 100
                horizontalAlignment: HusText.AlignRight
                text: "应用主题："
            }
            HusSelect {
                id: themeMode

                Layout.fillWidth: true
                animationEnabled: false
                displayText: label
                model: [
                    {
                        'label': qsTr('浅色'),
                        'value': HusTheme.Light
                    },
                    {
                        'label': qsTr('深色'),
                        'value': HusTheme.Dark
                    },
                    {
                        'label': qsTr('跟随系统'),
                        'value': HusTheme.System
                    }
                ]

                Component.onCompleted: {
                    currentIndex = ConfigManager.themeMode;
                    HusTheme.darkMode = currentIndex;
                }
                onActivated: {
                    console.log(currentValue);
                    HusTheme.darkMode = currentValue;
                }
            }
        }
        RowLayout {
            HusButton {
                text: "返回"
                width: 80
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop();
                    }
                }
            }
            HusButton {
                text: "保存"
                width: 80
                onClicked: saveConfig()
            }
        }
    }
}
