import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: trade

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50

            spacing: 5


            Text {
                Layout.fillHeight: true
                Layout.preferredWidth: implicitWidth

                text: "Current BTCUSDT: "
                font.pointSize: 15

            }
            Text {
                id: f_price
                Layout.fillHeight: true
                Layout.preferredWidth: implicitWidth


                font.pointSize: 15
                font.bold: true

            }
            Item {
                Layout.fillWidth: true
            }

        }

        Item {
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1
        }
    }

    Connections {
        target: websocketManager
        function onUpdateBTC(price) {
            f_price.text = String(price)
        }
    }

}
