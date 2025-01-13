pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: app

    states: [
        State {
            name: "ACC"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(acc);
                }
            }
        },
        State {
            name: "TRD"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(trade);
                }
            }
        },
        State {
            name: "ORD"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(orders);
                }
            }
        },
        State {
            name: "AST"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(assets);
                }
            }
        },
        State {
            name: "STG"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(setting);
                }
            }
        }
    ]

    state: "ACC"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        spacing:5

        AppSelector {
            id: selector
            Layout.fillWidth: true
            Layout.preferredHeight: 50

        }

        StackView {
            id: app_stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1
            initialItem: Item{}

            pushEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            pushExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
            popEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            popExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
        }
    }

    Connections {
        target: selector
        function onSelected(index){
            if (index === 0) {
                app.state = "ACC"
            } else if (index === 1) {
                app.state = "TRD"
            }  else if (index === 2) {
                app.state = "ORD"
            }  else if (index === 3) {
                app.state = "AST"
            }  else if (index === 4) {
                app.state = "STG"
            }
        }
    }

    Accounts {
        id: acc
        visible: false

    }

    Trade {
        id: trade
        visible: false

    }

    Item {
        id: orders
        visible: false
        Text {
            anchors.centerIn: parent
            text: "ORDERS"
        }
    }

    Item {
        id: assets
        visible: false
        Text {
            anchors.centerIn: parent
            text: "ASSETS"
        }
    }

    Item {
        id: setting
        visible: false
        Text {
            anchors.centerIn: parent
            text: "SETTING"
        }
    }
}
