import QtQuick 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

Item {
    
    property string title: 'Visiting history'
    
    anchors.fill: parent

    ColumnLayout {

        width:820
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        
        Text {
            text: "Visiting history"
        } 
        
        TextField {
            id: search
            height:10
            font.pixelSize: 10
            placeholderText: qsTr("Search history")
            background: Rectangle {
                implicitWidth: 600
                implicitHeight: 20
                opacity: enabled ? 1 : 0.3
                border.width: 1
                radius: 10
            }
            onTextChanged: listView.searchText = text
        }
        
        Component {
            id: rowDelegate
            Row {
                id: "row"
                required property string index
                required property string url
                required property string type
                required property string title
                required property string iconUrl
                required property string added
                required property string host
                width: 820
                height: 30
                Rectangle {
                    width: 800
                    height: parent.height
                    //CheckBox {
                        //width:16
                        //height:16

                        //id: modelCheckBoxes
                        //checked: model.checked
                        //text: model.number
                        //indicator.width: 18
                        //indicator.height: 18
                    //}
                    Text {
                        x: 30
                        height: 10
                        text: Qt.formatDateTime(added, "dd.MM hh:mm")
                    }
                    Image {
                        x:100
                        width:16
                        height:16
                        source: iconUrl
                    }
                    Text {
                        x: 120
                        height: 10
                        text: title + " | " + host
                        MouseArea {
                            anchors.fill: parent
                            onClicked: window.open(url)
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                    Button {
                        x:800
                        width:30
                        id: rowBtn
                        text: "..."
                        onClicked: menu.open()
                        background: Rectangle {
                            implicitWidth: 20
                            implicitHeight: 20
                            radius: 14
                            color: rowBtn.hovered ? "#bbbbbb" : "white"
                        }
                        Menu {
                            id: menu
                            y: rowBtn.height
                            background: Rectangle {
                                implicitWidth: 200
                                border.color: "#cccccc"
                                radius: 6
                            }

                            MenuItem {
                                text: "More from this site"
                                onTriggered: search.text = "host:" + row.host
                            }
                            MenuItem {
                                text: "Remove from history"
                                onTriggered: qi.visitHistoryModel().removeHistoryItem(row.index)
                            }
                        }
                    }

                }
            }
        } 
        
        
        SortFilterModel {
            id: delegateModel
            model: qi.visitHistoryModel()
            //model: Qqq
            delegate: rowDelegate
            
            filterAcceptsItem: function(item) {
                console.log(listView.searchText.substr(0, 5));
                if('host:' === listView.searchText.substr(0, 5)) {
                    return item.host.toLowerCase() == listView.searchText.toLowerCase().substr(5);
                }
                return item.title.toLowerCase().indexOf(listView.searchText.toLowerCase()) > -1;
            }
        }

        
        ListView {
            id: listView
            
            property string searchText: ''
            
            Layout.topMargin: 30
            height: parent.height
            spacing: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            // model: qi.visitHistoryModel()
            model: delegateModel
            //delegate: rowDelegate
            
            onSearchTextChanged: delegateModel.update()
            
            
            //ScrollBar.vertical: ScrollBar {
                //id: verticalScrollBar
                //active: true
                //orientation: Qt.Vertical
                //opacity: active ? 1:0
                //Behavior on opacity {NumberAnimation {duration: 500}}

                //contentItem: Rectangle {
                    //implicitWidth: 4
                    //radius:2
                    //implicitHeight: parent.height
                    //color: 'green'
                //}
            //}
        }
    }
}
