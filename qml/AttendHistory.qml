import QtQuick
import QtQuick.Layouts
import HuskarUI.Basic
import Fram

Item {
    id: attend_info

    property var stackView: null

    Connections{
        target: searchPage
        function onQueryCkicked(startTime, endTime, name){
            console.log(startTime,endTime,name)
            attendHistoryDto.queryAttendHistory(startTime, endTime, name);
        }
    }

    Component.onCompleted: {
        // 后端会默认当前0点到第二天0点的所有人
        attendHistoryDto.queryAttendHistory("","", "");
        CameraManager.videoOutput = null
    }

    AttendHistoryDto{
        id:attendHistoryDto
        onQueryReady: function(attendHistory){
            // 处理查询结果
            attendTable.initModel = attendHistory
            console.log(attendHistory[0]["name"])
            console.log("Query ready, got", attendHistory.length, "items");
            // 更新界面...
            // busyIndicator.running = false;
        }
        onQueryError: function(errorInfo){
            console.error("Query error:", errorInfo);
            // 显示错误信息...
            // busyIndicator.running = false;
        }
        onIsQueryingChanged: {
            // busyIndicator.running = attendHistoryDto.isQuerying;
        }
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20
        RowLayout{
            id:queryBtn
            HusIconButton {
                type: HusButton.Type_Primary
                iconSource: HusIcon.SearchOutlined
                Layout.preferredWidth: 35
                onClicked: {
                    searchPage.open()
                }
            }
            HusIconButton {
                type: HusButton.Type_Primary
                iconSource: HusIcon.ArrowLeftOutlined
                Layout.preferredWidth: 35
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop();
                    }
                }
            }
        }

        HusTableView {
            id: attendTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: [
                {
                    title: '姓名',
                    dataIndex: 'name',
                    width: 100,
                    minimumWidth: 100,
                    maximumWidth: 400,
                    align: 'center',
                    delegate: textDelegate,
                    selectionType: 'checkbox'
                },
                {
                    title: '工号',
                    dataIndex: 'staffNo',
                    width: 60,
                    delegate: textDelegate,
                    editable: true
                },
                {
                    title: '打卡时间',
                    dataIndex: 'attendTime',
                    width: 150,
                    delegate: textDelegate
                },
                {
                    title: '图片',
                    dataIndex: 'picUrl',
                    delegate: imageDelegate,
                    width: 80
                },
            ]
            minimumRowHeight: 50
        }

        HusDrawer {
            id: searchPage
            signal queryCkicked(string startTime, string endTime, string name)
            edge:Qt.BottomEdge
            titleDelegate: null
            closeDelegate: null
            contentDelegate:drawerContentDelegate
        }

        Component
        {
            id:drawerContentDelegate
            Item{
                ColumnLayout{
                    anchors.fill: parent  // 填充父Item
                    anchors.margins: 20
                    spacing: 10  // 可选：设置子项间距
                    HusDateTimePicker{
                        id:startTimerPicker
                        Layout.fillWidth: true
                        placeholderText: qsTr('请选择日期时间')
                        format: qsTr('yyyy-MM-dd hh:mm:ss')
                    }

                    HusDateTimePicker{
                        id:endTimerPicker
                        Layout.fillWidth: true
                        placeholderText: qsTr('请选择日期时间')
                        format: qsTr('yyyy-MM-dd hh:mm:ss')

                    }
                    HusInput{
                        id:nameInput
                        Layout.fillWidth: true
                        placeholderText:qsTr('请输入姓名')
                    }
                    RowLayout{
                        HusButton{
                            id:resetBtn
                            text:qsTr('重置')
                        }
                        HusButton{
                            id:queryBtn
                            text:qsTr('查询')
                            onClicked: {
                                searchPage.queryCkicked(
                                            Qt.formatDateTime(startTimerPicker.currentDateTime, "yyyy-MM-ddThh:mm:ss"),
                                            Qt.formatDateTime(endTimerPicker.currentDateTime, "yyyy-MM-ddThh:mm:ss"),
                                            nameInput.text
                                            )
                                searchPage.close()
                            }
                        }
                    }
                    Item{
                        Layout.fillHeight: true
                    }
                    Component.onCompleted: {
                        let now = new Date();
                        let todayStart = new Date(now.getFullYear(), now.getMonth(), now.getDate());
                        let tomorrowStart = new Date(todayStart.getTime() + 24 * 60 * 60 * 1000);
                        startTimerPicker.setDateTime(todayStart)
                        endTimerPicker.setDateTime(tomorrowStart)
                    }
                }
            }
        }

        Component {
            id: imageDelegate
            HusImage {
                fillMode: HusImage.PreserveAspectFit
                source: "file:" + cellData
            }
        }

        Component {
            id: textDelegate
            HusText {
                text: cellData
                color: HusTheme.Primary.colorTextBase
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                leftPadding: 8
                rightPadding: 8
            }
        }
    }
}
