import QtQuick
import QtQuick.Layouts
import HuskarUI.Basic
import Fram

Item {
    id: registerInfo
    property var stackView: nul
    Component.onCompleted: {
        // 后端会默认当前0点到第二天0点的所有人
        registerInfoDto.queryRegisterInfo();
        CameraManager.videoOutput = null
    }

    RegisterInfoDto{
        id:registerInfoDto
        onQueryReady: function(registerInfos){
            // 处理查询结果
            registerInfoTable.initModel = registerInfos
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
                iconSource: HusIcon.ReloadOutlined
                Layout.preferredWidth: 35
                onClicked: {
                    registerInfoDto.queryRegisterInfo();
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
            HusIconButton {
                type: HusButton.Type_Primary
                iconSource: HusIcon.UserAddOutlined
                Layout.preferredWidth: 35
                onClicked: {
                    stackView.push(registerComponent)
                }
            }

            HusIconButton {
                type: HusButton.Type_Primary
                iconSource: HusIcon.UserDeleteOutlined
                Layout.preferredWidth: 35
                onClicked: {
                    console.log("delete staff clicked")
                }
            }
        }


        HusTableView {
            id: registerInfoTable
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
                    title: '注册时间',
                    dataIndex: 'registerTime',
                    width: 150,
                    delegate: textDelegate
                },
                {
                    title: '图片',
                    dataIndex: 'picUrl',
                    delegate: imageDelegate,
                    width: 100
                },
            ]
            minimumRowHeight: 50
        }

        Component{
            id:registerComponent
            RegisterPage{
                stackView: registerInfo.stackView
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
                id: displayText
                color: HusTheme.Primary.colorTextBase
                horizontalAlignment: Text.AlignHCenter
                leftPadding: 8
                rightPadding: 8
                text: cellData
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
                    height: parent.height
                    opacity: 0.1
                    width: filterWidth.advanceWidth
                    x: startWidth.advanceWidth + (displayText.width - displayWidth.advanceWidth) * 0.5
                }
            }
        }

    }
}
