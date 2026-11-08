import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore

Image {
    anchors.fill: parent
    property string title: 'New Tab'

    id: root

    Settings {
        id: background
        property alias source: root.source
        property alias fillMode: root.fillMode
    }

    RoundButton {
        icon.source: "picture.svg"
        flat: true
        x: parent.width - width - 10
        y: parent.height - height - 10
        radius: 90
        onClicked: imagePopup.open()
    }

    Popup {
        id: imagePopup
        anchors.centerIn: parent
        width: parent.width * 0.7
        height: 100
        closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
        Column {
            anchors.fill: parent
            Row {
                spacing: 5
                Text { text: "Selected image: " + background.source; anchors.verticalCenter: parent.verticalCenter}
                Button { text: "Select another..."; onClicked: fileDialog.open(); anchors.verticalCenter: parent.verticalCenter}
            }
            Row {
                spacing: 5
                Text { text: "Image fill mode:"; anchors.verticalCenter: parent.verticalCenter}
                ComboBox {
                    model: [
                        {value: Image.Stretch, text: "Stretch"},
                        {value: Image.PreserveAspectFit, text: "Fit"},
                        {value: Image.PreserveAspectCrop, text: "Crop"},
                        {value: Image.Tile, text: "Tile"},
                        {value: Image.TileHorizontally, text: "Tile (horizontally)"},
                        {value: Image.TileVertically, text: "Tile (vertically)"},
                        {value: Image.Pad, text: "None"}
                    ]
                    textRole: "text"
                    valueRole: "value"
                    currentIndex: indexOfValue(root.fillMode)
                    onActivated: root.fillMode = currentValue
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Row {
        anchors.centerIn: parent
        height: 20
        TextField {
            id: searchField
            width: root.width * 0.7
            background.opacity: 0.6
            placeholderText: "Search something or write an URL..."
            onAccepted: {window.open(text, "_blank")}
        }
        Button {
            icon.source: "search.svg"
            onClicked: {window.open(searchField.text, "_blank")}
        }
    }

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: root.source = selectedFile
    }
}
