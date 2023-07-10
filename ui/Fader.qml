import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

ColumnLayout {
    id: columnLayout
    property int pinNumber
    property alias text: faderLabel.text
    property alias cc_value: spinBox.value
    property alias displayedValue: fader.value
    // signal ccValueChanged(int pin, int value)
    signal ccValueChanged(int pin, int value)

    Label {
        id: faderLabel
        text: ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        Layout.fillWidth: true
        color: "white"
    }

    Slider {
        id: fader
        width: 200
        height: 205
        stepSize: 1
        to: 127
        from: 1
        orientation: Qt.Vertical
        scale: 1
        Layout.fillHeight: true
        Layout.fillWidth: true
        rotation: 0
        value: 1
    }

    SpinBox {
        id: spinBox
        Layout.fillWidth: false
        wheelEnabled: true
        editable: true
        to: 127
        from: 1
        value: 0
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        onValueModified: {
            ccValueChanged(pinNumber, value);
        }
    }
}


