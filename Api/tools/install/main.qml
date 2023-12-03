import QtQuick 6.3
import QtQuick.Layouts 6.3
import QtQuick.Controls 6.3

Item {
    anchors.fill: parent
    property string title: 'Install'

    ColumnLayout {
        spacing: 20
        width: parent.width
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qi.progressInfo.url
            font.pixelSize: 20
            font.weight: Font.DemiBold
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Installing..."
            font.pixelSize: 20
            font.weight: Font.DemiBold
        }
        ProgressBar {
            Layout.alignment: Qt.AlignHCenter
            value: qi.progress / 100
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qi.progress
            font.pixelSize: 20
            font.weight: Font.DemiBold
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qi.progressStatus
            font.pixelSize: 20
            font.weight: Font.DemiBold
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            enabled: qi.progress = 1
            text: "Ok"
            onClicked: { 
                console.log(qi.progressInfo.dappUrl);
                document.location.href = qi.progressInfo.dappUrl; 
                
            }
        }
    }
}
