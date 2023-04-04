import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    
    property string title: 'Settings'
    anchors.fill: parent
        
    GridLayout {
        id: mainLayout
        columns: 2
        rowSpacing: 5
        columnSpacing: 5
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }

        Text {
            text: "Settings"
            font.pixelSize: 19
            anchors.bottomMargin: 100
            padding: 10
        }
        Text {}
        
        Label { 
            id: uaLabel
            text: "User Agent" 
        }
        TextField {
            id: appUserAgent
            text: qi.settingsModel().appUserAgent
            background: Rectangle {
                implicitWidth: parent.parent.parent.width - uaLabel.width - 100
                implicitHeight: 20
                opacity: enabled ? 1 : 0.3
                border.width: 1
                radius: 10
            }
            onTextChanged: qi.settingsModel().appUserAgent = text
        }
    
        Label { text: "Search Engine" }
        ComboBox {
            id: comboSE
            width: 200
            currentIndex: find(qi.settingsModel().browserSearchEngine)
            model: qi.settingsModel().browserSearchEngineList
            onCurrentIndexChanged: qi.settingsModel().browserSearchEngine = model[currentIndex]
            Component.onCompleted: {
                currentIndex = find(qi.settingsModel().browserSearchEngine);
            }
        }
        
        Label { text: "Cache Maximum Size" }
        TextField {
            id: appCacheMaxSize
            text: qi.settingsModel().appCacheMaxSize
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 20
                opacity: enabled ? 1 : 0.3
                border.width: 1
                radius: 10
            }
            onTextChanged: qi.settingsModel().appCacheMaxSize = text
            validator: IntValidator {bottom: 50; top: 10240}
        }
    }
}
