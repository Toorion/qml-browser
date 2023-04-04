import QtQuick 2.2
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15


Item {
    anchors.fill: parent
    property string title: 'Download history'

    ColumnLayout {

        width:820
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        
        Text {
            text: "DownloadList"
        } 

        Button {
            text: "Test"
            onClicked: console.log(parent.width)
        }

        
        Button {
            text: "Remove 3"
            onClicked: qi.downloadHistoryModel().removeDownloadItem(2)
        }

        Component {
            id: rowDelegate
            Row {
                required property string index
                required property string name
                required property string url
                required property string path
                required property string received
                width: 820
                height: 90
                //anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                    width: 800
                    height: parent.height
                    border.color: "#dcdcdc"
                    border.width: 1
                    radius: 10
                    Shape {
                        x: 10
                        width: 30
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        ShapePath {
                            fillColor: "#dcdcdc"
                            strokeColor: "#dcdcdc"
                            scale: Qt.size((1.3), (1.3))
                            PathSvg { path: "M6 2c-1.1 0-1.99.9-1.99 2L4 20c0 1.1.89 2 1.99 2H18c1.1 0 2-.9 2-2V8l-6-6H6zm7 7V3.5L18.5 9H13z" }
                        }
                    }

                    Rectangle {
                        id: icon_box
                        x: 50
                        width: 1
                        height: parent.height
                        color: "#dcdcdc"
                    }
                    Column {
                        anchors.left: icon_box.right
                        Text {
                            height: 5
                            text: " "
                        }
                        Text {
                            x: 5
                            height: 20
                            text: '<b>Name:</b> ' + name 
                        }
                        Text {
                            x: 5
                            height: 20
                            text: '<b>Url:</b> ' + url
                        }
                        Text {
                            x: 5
                            height: 20
                            text: '<b>Path:</b> ' + path
                        }
                        Text {
                            x: 5
                            height: 20
                            text: '<b>Size:</b> ' + received
                        }

                    }
                    Button {
                        id: del_item
                        anchors.right: parent.right
                        anchors.rightMargin: 2
                        anchors.top: parent.top
                        anchors.topMargin: 2
                        width: 30
                        height: 30
                        icon.source: "icon_clear.svg"
                        icon.width: 20
                        icon.height: 20
                        background: Rectangle {
                            border.width: 0
                            radius: 15
                            color: del_item.down ? "#999999" :(del_item.hovered?"#f0f0f0": "#ffffff")
                        }
                        onClicked: qi.downloadHistoryModel().removeDownloadItem(index)
                    }
                }
            }
        } 
        
        ListView {
            spacing: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: qi.downloadHistoryModel()
            delegate: rowDelegate
        }

    }
}
