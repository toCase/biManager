pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: app

    states: [
        State {
            name: "MAIN"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(acc_main);
                }
            }
        },
        State {
            name: "FORM"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(acc_form);
                }
            }
        },
        State {
            name: "HIST"
            PropertyChanges {}
            StateChangeScript {
                script: {
                    app_stack.pop();
                    app_stack.push(orders);
                }
            }
        }
    ]

    state: "MAIN"

    StackView {
        id: app_stack
        anchors.fill: parent
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

    AccountMain {
        id: acc_main
        visible: false
    }
    Connections {
        target: acc_main
        function onAdd() {
            acc_form.load(-1)
            app.state = "FORM"
        }
        function onOpen(i) {
            acc_form.load(i)
            app.state = "FORM"
        }
    }


    AccountForm {
        id: acc_form
        visible: false
    }
    Connections {
        target: acc_form
        function onCancel() {
            app.state = "MAIN"
        }
    }

    Item {
        id: trade
        visible: false
        Text {
            anchors.centerIn: parent
            text: "TRADE"
        }
    }

    Item {
        id: orders
        visible: false
        Text {
            anchors.centerIn: parent
            text: "ORDERS"
        }
    }

}
