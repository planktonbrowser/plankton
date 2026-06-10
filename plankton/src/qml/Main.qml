import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.plankton.kelp 1.0

Kirigami.ApplicationWindow {
    id: root

    TabBar {
        id: bar

        anchors.top: parent.top
        width: parent.width

        TabButton {
            text: qsTr("google.com")
        }

        TabButton {
            text: qsTr("libremelon.com")
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

    ServoWebView {
        url: "https://libremelon.com"
    }

}
