import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Controls"
import "const.js" as Const

Item {
    id: acc_main

    signal add();
    signal open(int i)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        spacing: 5

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: Const.row_but_height
            spacing: 5

            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }

            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 140
                text: "GET HISTORY"
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 160
                text: "ADD ACCOUNT"
                onClicked: acc_main.add()
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 140
                text: "UPDATE"
            }

            Item {
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
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        ListView {
            id: acc_view
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1

            model: modelAccount
            clip: true
            delegate: Item {

                required property int index
                required property bool m_selected
                required property string m_type
                required property string m_name
                required property string m_status
                required property string m_connected
                required property string m_balance

                width:  acc_view.width
                height: 40

                RowLayout {
                    anchors.fill: parent
                    spacing: 5

                    CheckBox {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 50
                        text: ""
                        checked: m_selected
                        onClicked: modelAccount.selectItem(index, checked)
                    }

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        text: m_type
                        verticalAlignment: Qt.AlignVCenter
                    }

                    Label {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.horizontalStretchFactor: 1

                        text: m_name
                        font.pointSize: 13
                        verticalAlignment: Qt.AlignVCenter
                    }
                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        text: m_status
                        color: Const.CLR_RED
                        verticalAlignment: Qt.AlignVCenter
                    }
                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        text: m_connected
                        color: Const.CLR_GREEN
                        verticalAlignment: Qt.AlignVCenter
                    }
                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        text: m_balance
                        verticalAlignment: Qt.AlignVCenter
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    anchors.leftMargin: 50
                    onDoubleClicked: acc_main.open(index)
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: Const.row_but_height
            spacing: 5
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 140
                text: "CONNECT"
                onClicked: modelAccount.connectItem()
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 140
                text: "DISCONNECT"
                onClicked: modelAccount.disconnectItem()
            }
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: Const.row_but_height
            spacing: 5
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 180
                text: "DISCONNECT ALL"
                onClicked: modelAccount.disconnectAll()
            }
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: Const.row_but_height
            spacing: 5
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
            MMT_Button {
                Layout.fillHeight: true
                Layout.preferredWidth: 140
                text: "DELETE"
            }
            Item {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
            }
        }
    }
}
