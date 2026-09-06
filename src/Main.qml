import org.kde.kirigami as Kirigami
import org.plankton.browser
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtWebEngine

Kirigami.ApplicationWindow {
    id: root

    TabBar {
        id: bar
        anchors.top: parent.top
        width: parent.width
        TabButton {
            text: qsTr("Google.com")
        }
        TabButton {
            text: qsTr("Bing.com")
        }
        TabButton {
            text: qsTr("kde.org")
        }
    }

    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex
        Item {
            id: tab1
        }
        Item {
            id: tab2
        }
        Item {
            id: tab3
        }
    }

    SearchField {
        id: search
        anchors.top: bar.bottom
        width: parent.width
    }

    // WebEngineView {
    //     anchors.top: search.bottom
    //     anchors.bottom: parent.bottom
    //     width: parent.width
    //     url: "https://libremelon.com"
    // }

    ServoWebView {
        id: servoView
        text: "Hello from C++"
        Component.onCompleted: servoView.doSomething()
    }
    Label {
        anchors.centerIn: parent
        text: servoView.text
    }
}
