import QtQuick
import QtQuick.Layouts

Item {

    property string title: 'Bookmarks'
    property string defaultFolderColor: "#ADBFBF"

    id: mainItem
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: '#e5f2f2'
        GridLayout {
            anchors.fill: parent
            columnSpacing: 0
            rowSpacing: 0

            LinksView {
                id: linksView
                z: 2
                Layout.column: 2
                Layout.preferredWidth: parent.width * 0.8
                Layout.fillHeight: true
            }

            Rectangle {
                z: 1
                Layout.column: 1
                Layout.preferredWidth: parent.width * 0.2
                Layout.fillHeight: true
                color: '#D5E9E9'
                FoldersView {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    id: foldersView
                    defaultColor: mainItem.defaultFolderColor
                }
            }
        }
    }

    Component.onCompleted: {
        let model = foldersView.model.findModelById(1);
        linksView.refresh(model);
    }
}
