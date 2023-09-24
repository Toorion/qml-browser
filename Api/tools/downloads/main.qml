import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes


Item {
    anchors.fill: parent
    property string title: 'Downloads'
    property int pixelSize: 16

    ColumnLayout {

        id: main
        width: window.width * 0.8
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        
        Text {
            text: "Downloads"
            font.pixelSize: pixelSize
            topPadding: 20
            bottomPadding: 20
        } 

        Component {
            id: rowDelegate
            Row {
                required property string index
                required property string name
                required property string url
                required property string path
                required property string received
                width: main.width
                height: 90
                Rectangle {
                    width: parent.width - 20
                    height: parent.height
                    border.color: "#dcdcdc"
                    border.width: 1
                    MouseArea {
                        id: rowMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.color = "#eee"
                        }
                        onExited: {
                            parent.color = "#fff"
                        }   
                        // onClicked: 
                        cursorShape: Qt.PointingHandCursor
                    }

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
                            color: del_item.down ? "#999999" :(del_item.hovered?"#bbb": "#fff")
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
