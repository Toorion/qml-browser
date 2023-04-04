import QtQuick 2.2
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {
    anchors.fill: parent
    property string title: 'Error page'
    Text {
        anchors.verticalCenter: parent.verticalCenter 
        anchors.horizontalCenter: parent.horizontalCenter 
        text: "Error page"
    }
}
