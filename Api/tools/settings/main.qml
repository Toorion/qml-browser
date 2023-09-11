import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    
    property string title: 'Settings'
    property int pixelSize: 16
    
    ListModel {
        id: styleList
        ListElement { name: "Basic" }
        ListElement { name: "Fusion" }
        ListElement { name: "Imagine" }
        ListElement { name: "Material" }
        ListElement { name: "Universal" }
    }
    
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
            font.pixelSize: 20
            anchors.bottomMargin: 100
        }
        Text {}
        
        Label { 
            text: "UI Style" 
            font.pixelSize: pixelSize
        }
        ComboBox {
            id: comboUIS
            currentIndex: find(qi.settingsModel().appStyle)
            model: styleList
            onCurrentIndexChanged: {
                if(qi.settingsModel().appStyle != model.get(currentIndex).name) {
                    qi.settingsModel().appStyle = model.get(currentIndex).name
                    alert("Restart the browser for the changes to take affect")
                }
                
            }
            Component.onCompleted: {
                currentIndex = find(qi.settingsModel().appStyle);
            }
            font.pixelSize: pixelSize
            implicitWidth: 200
        }

        
        Label { 
            id: uaLabel
            text: "User Agent"
            font.pixelSize: pixelSize
        }
        TextField {
            id: appUserAgent
            text: qi.settingsModel().appUserAgent
            implicitWidth: parent.parent.width - uaLabel.width - 120
            onTextChanged: qi.settingsModel().appUserAgent = text
            font.pixelSize: pixelSize
        }
    
        Label { 
            text: "Search Engine" 
            font.pixelSize: pixelSize
        }
        ComboBox {
            id: comboSE
            currentIndex: find(qi.settingsModel().browserSearchEngine)
            model: qi.settingsModel().browserSearchEngineList
            onCurrentIndexChanged: qi.settingsModel().browserSearchEngine = model[currentIndex]
            Component.onCompleted: {
                currentIndex = find(qi.settingsModel().browserSearchEngine);
            }
            font.pixelSize: pixelSize
            implicitWidth: 200
        }
        
        Label { 
            text: "Cache Maximum Size" 
            font.pixelSize: pixelSize
        }
        TextField {
            id: appCacheMaxSize
            text: qi.settingsModel().appCacheMaxSize
            onTextChanged: qi.settingsModel().appCacheMaxSize = text
            validator: IntValidator {bottom: 50; top: 10240}
            font.pixelSize: pixelSize
            implicitWidth: 200
        }
    }
}
