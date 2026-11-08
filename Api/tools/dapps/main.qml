import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    anchors.fill: parent
    property string title: "Dapp Manager"
    anchors.margins: 20
    Text {
        text: "Installed DAPPs: " + dappBridge.dapps.length
    }
    
    ListModel {
        id: dappsModel
        function reloadModel() {
            for (var i = 0; i < dappBridge.dapps.length; i++) {
                const dapp = dappBridge.dapps[i]
                const id = dapp
                const dot = dapp.indexOf('.')
                const name = dapp.slice(0, dot)
                const author = dapp.slice(dot + 1, dapp.indexOf('.', dot + 1))
                const source = dapp.slice(dapp.indexOf('.', dot + 1) + 1)
                dappsModel.append({
                    "id": id,
                    "name": name,
                    "author": author,
                    "source": source
                })
            }
        }
    }

    ListView {
        anchors.fill: parent
        model: dappsModel
        spacing: 15
        topMargin: 30
        bottomMargin: 10
        rightMargin: 10
        leftMargin: 10
        delegate: Rectangle {
            required property string name
            required property string id
            required property string author
            required property string source

            radius: 15
            height: 200
            width: parent.width
            border.color: "#030303"
            border.width: 4
            Column {
                anchors.fill: parent
                topPadding: 10
                bottomPadding: 10
                rightPadding: 10
                leftPadding: 10
                spacing: 10
                Text {text: name; font.bold: true; font.pointSize: 12}
                Text {text: "By: " + author}
                Text {text: "From: " + source}
                Text {text: id; font.italic: true}
                RowLayout {
                    spacing: 25
                    Button {
                        text: "Open"
                        onClicked: {
                            window.open("dapp://" + id)
                        }
                    }
                    Button {
                        text: "Update"
                        onClicked: {
                            window.open(source + "/" + author + "/" + name + ".git")
                        }
                    }
                }
            }
        }
    }
    
    Component.onCompleted: {
        dappBridge.refreshDapps();
        dappsModel.reloadModel();
    }
}