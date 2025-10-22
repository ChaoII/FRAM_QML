import QtQuick
import QtQuick.Layouts
import HuskarUI.Basic
import Fram

Item {
    id: registerInfo
    property var stackView: nul

    Component.onCompleted: {
        registerInfoDto.queryRegisterInfo();
        CameraManager.videoOutput = null
    }

    Connections{
        target: searchPage
        function onQueryCkicked(name){
            if(name){
                registerInfoDto.queryRegisterInfoByName(name);
                return
            }
            registerInfoDto.queryRegisterInfo();
        }
    }
    Connections {
        target: stackView
        function onCurrentItemChanged() {
            if (stackView.currentItem === registerInfo) {
                registerInfoDto.queryRegisterInfo();
            }
        }
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
        onDeleteFinished: function(){
            registerInfoDto.queryRegisterInfo();
        }

        onDeleteError: function(){
            console.error("Query error:", errorInfo);
        }

        onIsBusyChanged: {
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
                    let keys = registerInfoTable.getCheckedKeys()
                    registerInfoTable.clearAllCheckedKeys()
                    registerInfoDto.deleteRegisterInfos(keys);
                }
            }
        }

        HusDrawer {
            id: searchPage
            signal queryCkicked(name:string)
            titleDelegate: null
            closeDelegate: null
            edge:Qt.BottomEdge
            contentDelegate:Component
            {
                Item{
                    ColumnLayout{
                        anchors.fill: parent  // 填充父Item
                        anchors.margins: 20
                        spacing: 10  // 可选：设置子项间距
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
                                    searchPage.queryCkicked(nameInput.text)
                                    searchPage.close()
                                }
                            }
                        }
                        Item{
                            Layout.fillHeight: true
                        }
                    }
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
                    width: 180,
                    delegate: textDelegate
                },
                {
                    title: '图片',
                    dataIndex: 'picUrl',
                    delegate: imageDelegate,
                    width: 60
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
            }
        }
    }
}
