import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

Window {
    Universal.accent: Universal.Red
    Universal.theme: Universal.Dark
    color: Universal.Dark
    height: 400
    maximumHeight: 400
    maximumWidth: 600
    minimumHeight: 400
    minimumWidth: 600
    title: "Prometheus MIDI controller"
    visible: true
    width: 600

    Component.onCompleted: {
        backend.importsMidiDevices();
        backend.getAvailableSerialPorts();
    }

    ColumnLayout {
        spacing: 24
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 24

        ComboBox {
            id: midiDevices

            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            displayText: "Select MIDI port"
            editable: false
            flat: false
            height: 32
            model: []
            textRole: ""
            width: parent.width
            onActivated: {
                backend.setMidiDevice(currentIndex);
                midiDevices.displayText = currentValue;
            }
        }
        Switch {
            id: connectionButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Disconnected")

            onCheckedChanged: {
                backend.toggleSerial();
            }
        }
        RowLayout {
            id: gridLayout

            spacing: 8
            Fader {
                id: faderOne

                cc_value: 1
                pinNumber: 18
                text: "Modulation"

                onCcValueChanged: function (pin, value) {
                    backend.changeFaderCc(pin, value);
                }
            }
            Fader {
                id: faderTwo

                cc_value: 11
                pinNumber: 19
                text: "Expression"

                onCcValueChanged: function (pin, value) {
                    backend.changeFaderCc(pin, value);
                }
            }
            Fader {
                id: faderThree

                cc_value: 2
                pinNumber: 20
                text: "Vibrato"

                onCcValueChanged: function (pin, value) {
                    backend.changeFaderCc(pin, value);
                }
            }
            Fader {
                id: faderFour

                cc_value: 3
                displayedValue: 0
                pinNumber: 21
                text: "Custom"

                onCcValueChanged: function (pin, value) {
                    backend.changeFaderCc(pin, value);
                }
            }
        }
    }
    Connections {
        function onGetCCName(pin, value) {
            switch (true) {
            case pin === 18:
                faderOne.text = value;
                break;
            case pin === 19:
                faderTwo.text = value;
                break;
            case pin === 20:
                faderThree.text = value;
                break;
            case pin === 21:
                faderFour.text = value;
                break;
            default:
                return null;
            }
        }
        function onGetCCValue(pin, value) {
            switch (true) {
            case pin === 18:
                faderOne.displayedValue = value;
                break;
            case pin === 19:
                faderTwo.displayedValue = value;
                break;
            case pin === 20:
                faderThree.displayedValue = value;
                break;
            case pin === 21:
                faderFour.displayedValue = value;
                break;
            default:
                return null;
            }
        }
        function onGetIsSerialConnected(isConnected) {
            connectionButton.text = isConnected ? "Connected!" : "Disconnected";
        //            TODO: toggle off when not connected
        }
        function onGetMidiDevices(devices) {
            midiDevices.model = devices;
        }

        target: backend
    }
}
