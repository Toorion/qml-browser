import QtQuick 6.3
import QtQuick.Layouts
import QtQuick.Controls 2.12

Rectangle {
    id: target
    property int minSize: 10;
    property alias dragDrop: dragDrop

    signal resizeReleased()

    MouseArea {
        id: dragDrop
        x: 3
        y: 3
        width: parent.width - 6
        height: parent.height - 6
        drag.target: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
    }

    Item {
        id: left
        anchors {
           horizontalCenter: parent.left
           verticalCenter: parent.verticalCenter
        }
        //color: "green"
        width: 3
        height: target.height - 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor
            onMouseXChanged: {
                target.x = target.x + mouseX
                target.width = target.width - mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: right
        anchors {
           horizontalCenter: parent.right
           verticalCenter: parent.verticalCenter
        }
        //color: "green"
        width: 3
        height: target.height - 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor
            onMouseXChanged: {
                target.width = target.width + mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: top
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.top
        }
        //color: "green"
        width: target.width - 6
        height: 3
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeVerCursor
            onMouseYChanged: {
                target.y = target.y + mouseY
                target.height = target.height - mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: bottom
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.bottom
        }
        //color: "green"
        width: target.width - 6
        height: 3
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeVerCursor
            onMouseYChanged: {
                target.height = target.height + mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }


    //
    // Corners
    //

    Item {
        id: left_top
        anchors {
           horizontalCenter: parent.left
           verticalCenter: parent.top
        }
        //color: "red"
        width: 6
        height: 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            onMouseXChanged: {
                target.x = target.x + mouseX
                target.width = target.width - mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
                target.y = target.y + mouseY
                target.height = target.height - mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: right_top
        anchors {
           horizontalCenter: parent.right
           verticalCenter: parent.top
        }
        //color: "red"
        width: 6
        height: 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor
            onMouseXChanged: {
                target.width = target.width + mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
                target.y = target.y + mouseY
                target.height = target.height - mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: left_bottom
        anchors {
           horizontalCenter: parent.left
           verticalCenter: parent.bottom
        }
        //color: "red"
        width: 6
        height: 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeBDiagCursor
            onMouseXChanged: {
                target.x = target.x + mouseX
                target.width = target.width - mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
                target.height = target.height + mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

    Item {
        id: right_bottom
        anchors {
           horizontalCenter: parent.right
           verticalCenter: parent.bottom
        }
        //color: "red"
        width: 6
        height: 6
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            onMouseXChanged: {
                target.width = target.width + mouseX
                if(target.width < minSize)
                {
                    target.width = minSize
                }
                target.height = target.height + mouseY
                if(target.height < minSize)
                {
                    target.height = minSize
                }
            }
            onReleased: {
                target.resizeReleased()
            }
        }
    }

}
