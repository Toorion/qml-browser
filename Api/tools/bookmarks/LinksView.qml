import QtQuick 6.3
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: linksView

    property var bookmarkGroup: null

    function refresh(bookmark) {
        bookmarkGroup = bookmark;
        linksDelegateModel.update();
    }

    Rectangle {

        anchors.fill: parent
        color: 'plum'
        visible: bookmarkGroup !== null

        RowLayout {
            id: searchBox
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            Text {
                Layout.rightMargin: 20
                text: bookmarkGroup === null ? "" : bookmarkGroup.name
            }

            TextField {
                y: 20
                id: search
                placeholderText: qsTr("Search")
                implicitWidth: parent.parent.width * 0.6
                //anchors.horizontalCenter: parent.horizontalCenter
                onTextChanged: listView.searchText = text
            }
        }

        Component {
            id: linkDelegate
            Rectangle {
                required property int id
                required property string idx
                required property string link
                required property string title
                required property string icon
                required property string added
                width: linksView.width * 0.9
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
                Image {
                    y:4
                    width:parent.height - 4
                    height:parent.height - 4
                    source: icon
                }
                Text {
                    x: 30
                    height: parent.height
                    text: title
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
                            onTriggered: qi.visitHistoryModel().removeHistoryItem(row.idx)
                        }
                    }
                }
            }
        }


        SortFilterModel {
            id: linksDelegateModel
            model: qi.bookmarkLinkModel()
            delegate: linkDelegate

            filterAcceptsItem: function(item) {
                console.log(item.id);
                if (bookmarkGroup !== null && item.bookmarkId != bookmarkGroup.id) {
                    return;
                }

                //console.log(listView.searchText.substr(0, 5));
                if('host:' === listView.searchText.substr(0, 5)) {
                    return item.host.toLowerCase() == listView.searchText.toLowerCase().substr(5);
                }
                return item.title.toLowerCase().indexOf(listView.searchText.toLowerCase()) > -1;
            }
        }


        ListView {
            id: listView

            anchors.top: searchBox.bottom
            anchors.topMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40
            implicitWidth: parent.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter


            property string searchText: ''

            spacing: 2
            model: linksDelegateModel


            onSearchTextChanged: linksDelegateModel.update()

        }


    }
}
