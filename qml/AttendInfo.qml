import QtQuick
import QtQuick.Layouts
import HuskarUI.Basic

Item {
    id: attend_info
    height: 160

    property bool isUnknown: true
    property alias baseImageSource: baseImage.source
    property alias curImageSource: currentImage.source
    property alias name: nameText.text
    property alias staffNo: staffNo.text
    property alias attendTime: attendTime.text

    Rectangle {
        anchors.fill: parent
        color: isUnknown ? "#45FF0000" : "#4500FF00"

        RowLayout {
            anchors.fill: parent
            spacing: 10  // 元素间间距
            // 左侧弹簧
            Item {
                Layout.fillWidth: true
            }
            HusImage {
                id: baseImage
                Layout.preferredWidth: 120
                Layout.preferredHeight: 150
                previewEnabled: false
            }

            HusImage {
                id: currentImage
                Layout.preferredWidth: 120
                Layout.preferredHeight: 150
                previewEnabled: false
                fillMode: HusImage.PreserveAspectFit
            }

            GridLayout {
                id: grid
                columns: 2
                rowSpacing: 0
                columnSpacing: 0

                // 姓名
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.minimumWidth: 80
                    HusText {

                        text: "姓名:"
                        anchors.fill: parent
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                }
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    HusText {
                        id: nameText
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        text: "艾超"
                    }
                }

                // 打开时间
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.minimumWidth: 80
                    Layout.fillHeight: true
                    HusText {
                        text: "工号:"
                        anchors.fill: parent
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                }
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    HusText {
                        id: staffNo
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        text: "HR12138"
                    }
                }
                // 打开时间
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.minimumWidth: 80
                    Layout.fillHeight: true
                    HusText {
                        text: "打卡时间:"
                        anchors.fill: parent
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                }
                Rectangle {
                    border.width: 1
                    border.color: "gray"
                    color: "transparent"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    HusText {
                        id: attendTime
                        anchors.leftMargin: 10
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "2025-15-43 17:20:36"

                    }
                }

            }


            HusText {
                id: logInfo
                width: 120
                text: "右边坐标: " + (baseImage.x + baseImage.width)
                Layout.alignment: Qt.AlignVCenter
            }

            HusButton {
                id: ccc
                text: "点击"
                Layout.alignment: Qt.AlignVCenter
                onClicked: {
                    // 显示 baseImage 的右边界
                    let rightX = baseImage.x + baseImage.width
                    logInfo.text = "右边坐标: " + rightX
                    console.log("baseImage右边界:", rightX)
                }
            }

            // 左侧弹簧
            Item {
                Layout.fillWidth: true
            }
        }
    }
    onIsUnknownChanged:{
        console.log("------------------------------------------------------")
        console.log("isUnknownChanged:", isUnknown)
    }
}
