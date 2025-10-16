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
            id: row
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
                rowSpacing: 30
                columnSpacing: 10
                anchors.margins: 10

                property int fontSize: 16

                HusText {
                    text: "姓名:"
                    font.pixelSize: grid.fontSize
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }

                HusText {
                    id: nameText
                    font.pixelSize: grid.fontSize
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    text: ""
                }

                HusText {
                    text: "工号:"
                    font.pixelSize: grid.fontSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
                HusText {
                    id: staffNo
                    font.pixelSize: grid.fontSize
                    verticalAlignment: Text.AlignVCenter
                    text: ""
                }
                // 打开时间
                HusText {
                    text: "打卡时间:"
                    font.pixelSize: grid.fontSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
                HusText {
                    id: attendTime
                    font.pixelSize: grid.fontSize
                    verticalAlignment: Text.AlignVCenter
                    text: ""
                }
            }
            // 左侧弹簧
            Item {
                Layout.fillWidth: true
            }
        }
    }
    onIsUnknownChanged: {
        if (isUnknown) {
            row.visible = false
            return
        }
        row.visible = true
    }
}
