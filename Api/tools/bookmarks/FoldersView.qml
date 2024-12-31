import QtQuick 6.3
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Item {

    property string defaultColor
    property var model: bkModel

    Layout.fillWidth: true
    Layout.fillHeight: true

    MessageDialog {
        property var folderModel
        id: deleteConfirm
        text: "Deleting a folder " + (folderModel ? folderModel.name : "")
        informativeText: "Do you confirm?"
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            qi.bookmarkModel().remove(folderModel.id);
        }
    }

    MessageDialog {
        property var folderModel
        id: deleteNoEmpty
        text: "Deleting a folder" + (folderModel ? folderModel.name : "")
        informativeText: "Folder not empty, all links will be moved to Unsorted\nIgnore - Delete anyway"
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            qi.bookmarkLinkModel().updateLinksBookmark(folderModel.id, 1);
            let links = qi.bookmarkLinkModel().count("folder_links", {"bookmarkId":1});
            let idx = bkModel.findModelIndexById(1);
            bkModel.setProperty(idx, "links", links);
            qi.bookmarkModel().remove(folderModel.id);
        }
    }

    FolderForm {
        id: folderForm
        defaultFolderColor: mainItem.defaultFolderColor
    }

    Component {
        id: dragDelegate
        MouseArea {
            id: dragArea
            readonly property real indentation: 14
            readonly property real _padding: 5
            property bool held: false

            implicitHeight: 40
            anchors {
                left: parent?.left
                right: parent?.right
            }

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressed: {
                for(var i = 0; i < treeView.count; i++){ treeView.currentIndex = i; treeView.currentItem.z = i; }
                treeView.currentIndex = index
                if (treeView.currentItem != null) {
                    z = treeView.count
                }
                held = true
            }
            onReleased: {
                held = false;
                content.y = 0
                if (treeView.itemPosIndex != null) {
                    bkModel.move(treeView.sourceItemIndex, treeView.itemPosIndex);
                }
            }
            onDoubleClicked: {
                linksView.refresh(model);
            }
            onPositionChanged: (event) => {
                if (treeView.itemExitIndex != null) {
                    bkModel.move(treeView.sourceItemIndex,treeView.itemExitIndex);
                    treeView.itemExitIndex = null;
                    treeView.itemPosIndex = null;
                }
            }

            Rectangle {
                id: content
                clip: true
                implicitHeight: parent.height
                implicitWidth: parent.width

                color: model.color ? model.color : defaultColor

                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2
                Drag.keys: ["folder"]

                Menu {
                    id: folderMenu
                    MenuItem {
                        text: "Edit";
                        onTriggered: {
                            folderForm.edit(model);
                        }
                    }
                    MenuItem {
                        text: "Add subfolder";
                        onTriggered: {
                            folderForm.add(model.id);
                        }
                    }
                    MenuItem {
                        text: "Delete";
                        visible: model.id > 1
                        height: visible ? implicitHeight : 0
                        onTriggered: {
                            if (model.links > 0) {
                                deleteNoEmpty.folderModel = model
                                deleteNoEmpty.open();
                            } else {
                                deleteConfirm.folderModel = model
                                deleteConfirm.open();
                            }
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

                Label {
                    id: indicator
                    x: _padding + (model.depth * indentation)
                    anchors.verticalCenter: parent.verticalCenter
                    visible: model.childsCount > 0
                    text: model.expanded ? "–" : "▶"

                    TapHandler {
                       onSingleTapped: {
                           model.expanded = !model.expanded;
                           qi.bookmarkModel().save(model);
                           bkModel.update();
                       }
                    }
                }
                Label {
                    x: _padding + (model.depth + 1) * indentation
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - padding - x
                    clip: true
                    text: model.name
                }
                Label {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 40
                    clip: true
                    text: model.links ? model.links : 0
                }
            }

            DropArea {
                keys: ["link"]
                anchors {
                    fill: parent
                }
                onDropped: (drag) => {
                    let dm = drag.source.DelegateModel;
                    let link = dm.model.modelByIndex(dm.itemsIndex);
                    let newFolder = bkModel.modelByIndex(dragArea.DelegateModel.itemsIndex);
                    link.bookmarkId = newFolder.id;
                    qi.bookmarkLinkModel().save(link);
                    dm.model.update();
                    let oldFolder = qi.bookmarkModel().get(treeView.currentIndex);
                    qi.bookmarkModel().setProperty(treeView.currentIndex, "links", oldFolder.links-1);
                    bkModel.setProperty(dragArea.DelegateModel.itemsIndex, "links", newFolder.links+1);
                }
            }

            DropArea {
                keys: ["folder"]
                anchors {
                    fill: parent
                    margins: 10
                }
                onEntered: (drag) => {
                    treeView.sourceItemIndex = drag.source.DelegateModel.itemsIndex;
                }
                onExited: {
                    treeView.itemExitIndex = dragArea.DelegateModel.itemsIndex;
                }
                onPositionChanged: (drag) => {
                    treeView.itemPosIndex = dragArea.DelegateModel.itemsIndex;
                    treeView.itemExitIndex = null;
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

        property var itemExitIndex: null
        property var itemPosIndex: null
        property int sourceItemIndex: 0

        Menu {
            id: groupMenu
            MenuItem {
                text: "Add new folder";
                onTriggered: {
                    folderForm.add(0);
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
