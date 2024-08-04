import QtQuick 6.3
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import "app.js" as App

Item {
    id: mainItem
    anchors.fill: parent
    property string title: 'Start'
    
    
    GridLayout {
        anchors.fill: parent
        columns: 2
        rows: 2
        columnSpacing: 0
        rowSpacing:0
        
        Rectangle {
            //Layout.fillHeight: true
            //Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.2
            Layout.preferredWidth: parent.width * 0.7
            color: "red"
        }
        
        Rectangle {
            //Layout.fillHeight: true
            //   Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.2
            Layout.preferredWidth: parent.width * 0.3

            color: "blue"
        }

        Rectangle {
            id: groupFrame
            clip: true
            //Layout.fillHeight: true
            //   Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.8
            Layout.preferredWidth: parent.width

            Layout.columnSpan: 2
            
            MouseArea {
               anchors.fill: parent
               acceptedButtons: Qt.RightButton
               onClicked: {
                   //parent.color = 'red';
                   addPopup.open();
               }
            }

            Popup {
                id: addPopup
                width: 300;
                height: 200
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                closePolicy: Popup.CloseOnEscape
                modal: true

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
                        currentColor: colorFld.color
                    }

                    Label { text: "Color" }
                    Rectangle {
                        id: colorFld
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
                                addPopup.close();
                            }
                        }

                        QbButton {
                            anchors.right: parent.right
                            //anchors.horizontalCenter: parent.horizontalCenter
                            //Layout.alignment: Qt.AlignRight
                            //x: parent.width
                            text: "Add"
                            onClick: function() {
                                let item = qi.bookmarkModel().create();
                                item.parentId = 0;
                                item.name = itemName.text;
                                item.color = colorFld.color;
                                qi.bookmarkModel().addItem(item);
                                addPopup.close();
                            }
                        }
                    }
                }

            }

            Repeater {
                model: SortFilterModel {
                    model: qi.bookmarkModel()
                    filterAcceptsItem: function(item) {
                        return item.parentId == 0;
                    }
                    delegate: ResizableBox {
                        x: groupFrame.width / 100 * model.x
                        y: groupFrame.height / 100 * model.y
                        width: Math.max(100, model.width)
                        height: Math.max(100, model.height)
                        color: model.color

                        onResizeReleased: {
                            console.log("ResRel");
                            model.width = width;
                            model.height = height;
                            qi.bookmarkModel().save();
                        }

                        dragDrop.onReleased: {
                            model.x = 100 / groupFrame.width * x;
                            model.y = 100 / groupFrame.height * y;
                            qi.bookmarkModel().save();
                        }
                        dragDrop.onClicked: function(mouse) {
                            if (mouse.button === Qt.RightButton)
                                contextMenu.open()
                        }

                        Menu {
                            id: contextMenu
                            MenuItem {
                                text: "Edit";
                                onTriggered: {
                                    itemName.text = model.name;
                                    colorFld.color = model.color;
                                    addPopup.open();
                                }
                            }
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: parent.color
                            clip: true
                            anchors.margins: 4

                            Text {
                                text: name
                            }

                            TreeView {
                                anchors.fill: parent
                                anchors.topMargin: 20
                                selectionModel: ItemSelectionModel {}
                                model: qi.bookmarkModel()
                                delegate: Item {
                                    implicitHeight: 20
                                    required property string name
                                    Text {
                                        text: name
                                    }
                                }
                            }

                        }
                    }
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
