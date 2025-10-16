import QtQuick
import QtQuick.Layouts
import HuskarUI.Basic

Item {
    id: attend_info
    property var stackView: null

    HusTableView {
        id: attendTable
        anchors.fill: parent
        minimumRowHeight:200

        columns: [
            {
                title: '姓名',
                dataIndex: 'name',
                width: 100,
                minimumWidth: 100,
                maximumWidth: 400,
                align: 'center',
                delegate: textDelegate,
                selectionType: 'checkbox',
            },
            {
                title: '工号',
                dataIndex: 'staffNo',
                width: 60,
                delegate: textDelegate,
                editable: true,
            },
            {
                title: '打卡时间',
                dataIndex: 'attendTime',
                width: 150,
                delegate: textDelegate,

            },
            {
                title: '图片',
                dataIndex: 'picUrl',
                delegate: imageDelegate,
                width: 100,
            },
        ]

    }

    Component {
        id: imageDelegate
        HusImage {
            source: "file:" + cellData
            fillMode: HusImage.PreserveAspectFit
        }
    }


    Component {
        id: textDelegate

        HusText {
            id: displayText
            leftPadding: 8
            rightPadding: 8
            text: cellData
            color: HusTheme.Primary.colorTextBase
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            TextMetrics {
                id: displayWidth
                font: displayText.font
                text: displayText.text
            }

            TextMetrics {
                id: startWidth
                font: displayText.font
                text: {
                    let index = displayText.text.indexOf(filterInput);
                    if (index !== -1)
                        return displayText.text.substring(0, index);
                    else
                        return '';
                }
            }

            TextMetrics {
                id: filterWidth
                font: displayText.font
                text: filterInput
            }

            Rectangle {
                color: 'red'
                opacity: 0.1
                x: startWidth.advanceWidth + (displayText.width - displayWidth.advanceWidth) * 0.5
                width: filterWidth.advanceWidth
                height: parent.height
            }
        }
    }




    Component.onCompleted: {

        attendTable.initModel = [
                    {
                        name: "张飞", staffNo: "HR0987", attendTime: "2025-10-16T14:28",
                        picUrl: "E:/qtcreator_p/fram_qml/build/attend/202510/16/20251016140611016_123.jpg"
                    },
                    {
                        name: "李四", staffNo: "HR0001", attendTime: "2025-10-10T14:28",
                        picUrl: "E:/qtcreator_p/fram_qml/build/attend/202510/16/20251016135848707_123.jpg"
                    },
                    {
                        name: "王五", staffNo: "HR0002", attendTime: "2025-10-10T14:03",
                        picUrl: "E:/qtcreator_p/fram_qml/build/attend/202510/16/20251016135255355_123.jpg"
                    }]

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
}
