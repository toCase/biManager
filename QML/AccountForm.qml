import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: acc_form

    signal cancel()

    property int id_acc: 0

    function load(i){
        if (i === -1){
            id_acc = 0
            f_name.clear()
            f_type.currentIndex = 0
            f_api.clear()
            f_secret.clear()

            f_name.focus = true
        } else {
            let card = modelAccount.getCard(i)
            id_acc = card.idx
            f_name.text = card.name
            f_type.currentIndex = f_type.find(card.type)
            f_api.text = card.api
            f_secret.text = card.secret
        }
        console.log("ID ACC: ", id_acc)
    }

    function save(){
        let card = {
            "id": id_acc,
            "name": f_name.text,
            "type": f_type.currentText,
            "api": f_api.text,
            "secret": f_secret.text
        }
        let r = modelAccount.saveItem(card)
        if (r){
            acc_form.cancel()
        }

    }


    ColumnLayout {
        // anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width < 400 ? parent.width : parent.width > 1000 ? parent.width * 0.6 : parent.width * 0.8
        height: implicitHeight

        spacing: 5

        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            font.pointSize: 15
            text: "Account"
            horizontalAlignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            spacing: 5

            Text {
                Layout.preferredWidth: Math.max(80, implicitWidth)
                Layout.fillHeight: true

                text: "NAME:"
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
            }
            TextField {
                id: f_name
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            spacing: 5

            Text {
                Layout.preferredWidth: Math.max(80, implicitWidth)
                Layout.fillHeight: true

                text: "TYPE:"
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
            }
            ComboBox {
                id: f_type
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
                model: ["bybit", "binance"]
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            spacing: 5

            Text {
                Layout.preferredWidth: Math.max(80, implicitWidth)
                Layout.fillHeight: true

                text: "API KEY:"
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
            }
            TextField {
                id: f_api
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            spacing: 5

            Text {
                Layout.preferredWidth: Math.max(80, implicitWidth)
                Layout.fillHeight: true

                text: "SECRET:"
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
            }
            TextField {
                id: f_secret
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            spacing: 5

            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }

            Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 120
                text: "SAVE"
                onClicked: acc_form.save()
            }
            Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 120
                text: "CANCEL"
                onClicked: acc_form.cancel()
            }
        }
    }

}
