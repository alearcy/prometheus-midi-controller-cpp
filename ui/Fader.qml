import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

ColumnLayout {
    id: columnLayout

    property alias cc_value: spinBox.value
    property alias displayedValue: fader.value
    property int pinNumber
    property alias text: faderLabel.text

    signal ccNameChanged(int pin, string ccName)
    signal ccValueChanged(int pin, int value)

    TextEdit {
        id: faderLabel

        Layout.fillWidth: true
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        selectionColor: "red"
        text: ""
        verticalAlignment: Text.AlignVCenter

        Keys.onReturnPressed: {
            faderLabel.focus = false;
            editingFinished();
        }
        onEditingFinished: {
            ccNameChanged(pinNumber, faderLabel.text);
        }
    }
    Slider {
        id: fader
        Layout.fillHeight: true
        Layout.fillWidth: true
        from: 1
        orientation: Qt.Vertical
        rotation: 0
        scale: 1
        stepSize: 1
        to: 127
        value: 1
    }
    SpinBox {
        id: spinBox

        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.fillWidth: false
        editable: true
        from: 1
        to: 127
        value: 0
        wheelEnabled: true

        onValueModified: {
            ccValueChanged(pinNumber, value);
        }
    }
}

