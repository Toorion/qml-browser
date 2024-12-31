import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    
    property string title: 'Visiting history'
    property int pixelSize: 16
    
    anchors.fill: parent

    ColumnLayout {

        id: main
        width: window.width * 0.8
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        
        Text {
            text: "Visiting history"
            font.pixelSize: pixelSize
            topPadding: 20
            bottomPadding: 20
        } 
        
        TextField {
            id: search
            placeholderText: qsTr("Search history")
            implicitWidth: parent.width
            onTextChanged: listView.searchText = text
        }
        
        Component {
            id: rowDelegate
            Rectangle {
                required property int index
                required property string idx
                required property string url
                required property string type
                required property string title
                required property string iconUrl
                required property string added
                required property string host
                width: main.width
                height: 30
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
                    onClicked: window.open(url)
                    cursorShape: Qt.PointingHandCursor
                }
                Text {
                    height: parent.height
                    text: Qt.formatDateTime(added, "dd.MM hh:mm")
                    verticalAlignment: Text.AlignVCenter
                }
                Image {
                    x:80
                    y:4
                    width:parent.height - 4
                    height:parent.height - 4
                    source: iconUrl
                }
                Text {
                    x: 110
                    height: parent.height
                    text: title + " | " + host
                    verticalAlignment: Text.AlignVCenter
                }
                Button {
                    id: rowBtn
                    width:30
                    x: parent.width - width
                    text: "..."
                    onClicked: menu.open()
                    background: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 20
                        color: rowBtn.hovered ? "#bbbbbb" : "#eee"
                    }
                    Menu {
                        id: menu
                        y: rowBtn.height
                        background: Rectangle {
                            implicitWidth: 200
                            border.color: "#cccccc"
                        }

                        MenuItem {
                            text: "More from this site"
                            onTriggered: search.text = "host:" + row.host
                        }
                        MenuItem {
                            text: "Remove from history"
                            onTriggered: qi.visitHistoryModel().removeHistoryItem(index)
                        }
                    }
                }
            }
        } 
        
        
        SortFilterModel {
            id: delegateModel
            model: qi.visitHistoryModel()
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
            
            Layout.topMargin: 20
            height: parent.height
            spacing: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: delegateModel
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
