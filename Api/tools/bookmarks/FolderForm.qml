import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Popup {
    id: folderForm
    width: 300;
    height: 200
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    closePolicy: Popup.CloseOnEscape
    modal: true

    property int parentId
    property var model
    property var modelIndex
    property string formAction

    function add(_model) {
        formAction = "add";
        model = _model;
        folderForm.open();
    }

    function edit(_model) {
        formAction = "edit";
        model = _model;
        itemName.text = model.name;
        colorDialog.color = model.color;
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
            width: 40
            height: 40
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
                    //console.log(qi.bookmarkModel().rowCount());
                    folderForm.close();
                }
            }

            QbButton {
                anchors.right: parent.right
                //anchors.horizontalCenter: parent.horizontalCenter
                //Layout.alignment: Qt.AlignRight
                //x: parent.width
                text: "Save"
                onClick: function() {
                    var item;
                    if(formAction === 'add') {
                        qi.bookmarkModel().add({parentId: model.id, name: itemName.text});
                    } else {
                        model.name = itemName.text;
                        model.color = itemColor.color;
                        qi.bookmarkTreeModel().submit();
                    }
                    folderForm.close();
                }
            }
        }
    }

}
