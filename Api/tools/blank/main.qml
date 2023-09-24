import QtQuick 2.2


Item {
    anchors.fill: parent
    property string title: 'Blank'
    Text {
        anchors.verticalCenter: parent.verticalCenter 
        anchors.horizontalCenter: parent.horizontalCenter 
        text: "Blank"
        font.pixelSize: window.height / 3
        font.weight: Font.DemiBold
        color: "#EEE"
    }
}
