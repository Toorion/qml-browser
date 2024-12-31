import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Popup {

    property string defaultFolderColor
    property int parentId
    property var model
    property var modelIndex
    property string formAction
    property var treeView

    id: folderForm
    width: 300;
    height: 200
    x: (mainItem.width - width) / 2
    y: (mainItem.height - height) / 2
    closePolicy: Popup.CloseOnEscape
    modal: true


    function add(_parentId) {
        parentId = _parentId;
        itemName.text = "";
        colorDialog.color = defaultFolderColor;
        formAction = "add";
        folderForm.open();
    }

    function edit(_model) {
        model = _model;
        itemName.text = model.name;
        colorDialog.color = model.color ? model.color : defaultFolderColor;
        formAction = "edit";
        folderForm.open();
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        rows: 3
        Label {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 22
            text: "Add folder"
        }

        Label { text: "Name" }
        TextField {
            id: itemName
            objectName: "itemName"
            Layout.fillWidth: true
        }

        ColorDialog {
            id: colorDialog
            currentColor: itemColor.color
        }

        Label { text: "Color" }
        Rectangle {
            id: itemColor
            implicitWidth: 40
            implicitHeight: 40
            color: colorDialog.color
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    colorDialog.open();
                }
            }
        }

        Rectangle {
            Layout.columnSpan: 2
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * 0.3
            Button {
                text: 'Cancel'
                onClicked: {
                    folderForm.close();
                }
            }

            QbButton {
                anchors.right: parent.right
                text: "Save"
                onClick: function() {
                    var item;
                    if(folderForm.formAction === 'add') {
                        qi.bookmarkModel().add({parentId: parentId, name: itemName.text, color: itemColor.color, depth: 0, idx: 0});
                    } else {
                        model.name = itemName.text;
                        model.color = itemColor.color;
                        qi.bookmarkModel().save(model);
                    }
                    folderForm.close();
                }
            }
        }
    }
}
