import QtQuick 6.3
import QtQuick.Controls 6.3
import QtQuick.Layouts 6.3
import "palette.js" as Palette

Button {
    property var onClick;
    Layout.alignment: Qt.AlignHCenter
    text: "Ok"
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        color:Palette.bgSuccess
    }
       
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onEntered: {
            if(parent.enabled) {
                parent.background.color = Palette.bgSuccessHover
            }
        }
        onExited: {
            if(parent.enabled) {
                parent.background.color = Palette.bgSuccess
            }
        }
        onClicked: {onClick();}
    }
}
