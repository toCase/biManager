// pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQml.Models
import QtQuick.Controls.Material

import "const.js" as Const

Item {
    id: selector

    signal selected(int doc_index)

    ListModel {
        id: slc_model
        ListElement { name: "Accounts"}
        ListElement { name: "Trade"}
        ListElement { name: "Orders"}
        ListElement { name: "Assets"}
        ListElement { name: "Setting"}
    }

    Component {
        id: highlight
        Rectangle {
            width: selector.width / 5;
            height: 40
            color: Const.CLR_ACCENT; radius: 5
            x: slc_list.currentItem.x
            Behavior on x {
                SpringAnimation {
                    spring: 10
                    damping: 0.8
                }
            }
        }
    }


    ListView {
        id: slc_list
        anchors.fill: parent
        model: slc_model
        highlight: highlight
        orientation: Qt.Horizontal
        delegate: Item {

            width: selector.width / 5
            height: 40

            Label {
                anchors.fill: parent
                text: name
                font.pointSize: 15
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    slc_list.currentIndex = index
                    selector.selected(index)
                }

            }

        }
        highlightFollowsCurrentItem: false
        focus: true

    }


}
