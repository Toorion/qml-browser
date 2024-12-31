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
        color: '#e5f2f2'
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
                onTextChanged: listView.searchText = text
            }
        }

        Component {
            id: linkDelegate
            MouseArea {
                id: dragLinkArea
                required property int id
                required property string idx
                required property string link
                required property string title
                required property string icon
                required property string added
                property bool held: false

                implicitHeight: 30
                anchors {
                    left: parent?.left
                    right: parent?.right
                }

                drag.target: held ? linkContent : undefined

                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onPressed: {
                    held = true
                }
                onReleased: {
                    linkContent.Drag.drop();
                    held = false;
                }
                onEntered: {
                    parent.color = "#eee"
                }
                onExited: {
                    parent.color = "#fff"
                }
                onClicked: window.open(link)

                Rectangle {
                    id: linkContent

                    Drag.active: dragLinkArea.held
                    Drag.source: dragLinkArea
                    Drag.keys: ["link"]

                    width: parent.width
                    height: parent.height
                    Image {
                        y: 2
                        x: 2
                        width:parent.height - 4
                        height:parent.height - 4
                        source: icon
                    }
                    Text {
                        x: 34
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
                                text: "Remove"
                                onTriggered: qi.bookmarkLinkModel().removeItem(idx)
                            }
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
                if (bookmarkGroup !== null && item.bookmarkId != bookmarkGroup.id) {
                    return;
                }

                if('host:' === listView.searchText.substr(0, 5)) {
                    return item.host.toLowerCase() == listView.searchText.toLowerCase().substr(5);
                }
                return item.text.toLowerCase().indexOf(listView.searchText.toLowerCase()) > -1;
            }

            lessThan: function(left, right) {
                return left.added < right.added;
            }
        }


        ListView {
            property string searchText: ''

            id: listView
            anchors.top: searchBox.bottom
            anchors.topMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40
            implicitWidth: parent.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 2
            model: linksDelegateModel
            onSearchTextChanged: linksDelegateModel.update()
        }
    }
}
