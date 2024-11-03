import QtQuick 6.3
import QtQuick.Layouts
import QtQuick.Controls
import "app.js" as App

Item {
    id: mainItem
    anchors.fill: parent
    property string title: 'Bookmarks'
    
    
    GridLayout {
        anchors.fill: parent
        columns: 2
        rows: 2
        columnSpacing: 0
        rowSpacing:0
        
        Rectangle {
            Layout.preferredHeight: parent.height * 0.2
            Layout.preferredWidth: parent.width * 0.7
            color: "red"
        }
        
        Rectangle {
            Layout.preferredHeight: parent.height * 0.2
            Layout.preferredWidth: parent.width * 0.3

            color: "blue"
        }

        Rectangle {
            id: groupFrame
            clip: true
            Layout.preferredHeight: parent.height * 0.8
            Layout.preferredWidth: parent.width
            Layout.columnSpan: 2
            
            FolderForm {
                id: folderForm
            }

            RowLayout {
                id: layout
                anchors.fill: parent
                spacing: 0
                Rectangle {
                    color: 'teal'
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumWidth: 50
                    Layout.preferredWidth: 100
                    Layout.maximumWidth: 300
                    Layout.minimumHeight: 150

                    Component {
                        id: dragDelegate
                        MouseArea {
                            id: dragArea
                            readonly property real indentation: 20
                            readonly property real padding: 5
                            property bool held: false

                            implicitHeight: 40
                            anchors {
                                left: parent?.left
                                right: parent?.right
                            }

                            drag.target: held ? content : undefined
                            drag.axis: Drag.YAxis

                            onPressed: held = true
                            onReleased: held = false
                            onDoubleClicked: {
                                linksView.refresh(model);
                            }

                            Rectangle {
                                id: content
                                clip: true
                                implicitHeight: parent.height
                                implicitWidth: treeView.width
                                color: model.color ? model.color : "#f0f0f0"


                                Drag.active: dragArea.held
                                Drag.source: dragArea
                                Drag.hotSpot.x: width / 2
                                Drag.hotSpot.y: height / 2

                                Label {
                                    id: indicator
                                    x: padding + (model.depth * indentation)
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: model.childsCount > 0
                                    text: "▶"

                                    TapHandler {
                                       onSingleTapped: {
                                           model.expanded = !model.expanded;
                                           bkModel.update();
                                       }
                                    }
                                }
                                Label {
                                    x: padding + (depth + 1) * indentation
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width - padding - x
                                    clip: true
                                    text: model.name

                                    Menu {
                                        id: folderMenu
                                        MenuItem {
                                            text: "Add child";
                                            onTriggered: {
                                                folderForm.add(model, treeView.index(row,column));
                                            }
                                        }

                                        MenuItem {
                                            text: "Edit";
                                            onTriggered: {
                                                folderForm.edit(model);
                                            }
                                        }
                                    }

                                    TapHandler {
                                        acceptedButtons: Qt.RightButton
                                        gesturePolicy: TapHandler.WithinBounds
                                        onSingleTapped: function(mouse){
                                            folderMenu.popup(mouse.x, mouse.y);
                                        }
                                    }

                                }
                                Label {
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 40
                                    clip: true
                                    text: model.childsCount
                                }

                            }

                            DropArea {
                               anchors {
                                   fill: parent
                                   margins: 10
                               }

                               onEntered: (drag) => {
                                   bkModel.items.move(
                                           drag.source.DelegateModel.itemsIndex,
                                           dragArea.DelegateModel.itemsIndex)
                               }
                           }

                        }
                    }

                    TreeFilterModel {
                        id: bkModel
                        model: qi.bookmarkModel()
                        delegate: dragDelegate
                    }

                    ListView {
                        id: treeView
                        anchors.fill: parent
                        anchors.margins: 0
                        clip: true
                        model: bkModel

                        Menu {
                            id: groupMenu
                            MenuItem {
                                text: "Add new folder";
                                onTriggered: {
                                    folderForm.add(bkModel);
                                }
                            }
                        }

                        TapHandler {
                            acceptedButtons: Qt.RightButton
                            onSingleTapped: function (mouse) {
                                groupMenu.popup(mouse.x, mouse.y);
                            }
                        }
                    }
                }

                LinksView {
                    id: linksView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }

    Connections {
        target: navInput
        function onValueChanged(text) {log.debug("TYPE" + text);}
    }

    //Component.onCompleted: App.init(console, document)



}
