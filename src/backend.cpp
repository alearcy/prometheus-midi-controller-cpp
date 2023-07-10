#include "backend.h"
#include <QDebug>
#include <QThread>
#include "RtMidi.h"
#include <QSerialPort>
#include <QByteArray>

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    // configure MIDI
    midiout = new RtMidiOut();
    // configure SERIAL
    serial = new QSerialPort();
    serialInfo = new QSerialPortInfo();
    serialPorts = serialInfo->availablePorts();
}

Backend::~Backend()
{
    delete midiout;
    delete serial;
}

void Backend::importsMidiDevices()
{
    std::vector<unsigned char> message;
    unsigned int nPorts = midiout->getPortCount();
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            m_midiDevices << QString::fromStdString(midiout->getPortName(i));
        }
        catch (RtMidiError &error) {
            error.printMessage();
        }
    }
    emit getMidiDevices(m_midiDevices);
}

void Backend::toggleSerial()
{
    if (serialToggled)
        emit getIsSerialConnected(false);
        serial->close();
    for (auto &serialPort : serialPorts) {
        if (serialPort.description() == "Arduino Micro")
            m_serialPortName = serialPort.portName();
    }

    // toggle serial connected
    if (serial->portName() != "")
        serial->open(QIODevice::ReadOnly);
    serial->setPortName(m_serialPortName);
    configureSerialPort();
    serial->open(QIODevice::ReadOnly);
    if (serial->isOpen())
        emit getIsSerialConnected(true);
        serialToggled = !serialToggled;
    QThread::msleep(5);
    connect(serial, SIGNAL(readyRead()), this, SLOT(receive()));
}

void Backend::setMidiDevice(unsigned int currentIndex)
{
    if (m_midiDevice != currentIndex)
        m_midiDevice = currentIndex;
        midiout->openPort(currentIndex);
}

void Backend::configureSerialPort()
{
    serial->setBaudRate(QSerialPort::BaudRate::Baud115200);
    serial->setParity(QSerialPort::Parity::NoParity);
    serial->setDataBits(QSerialPort::DataBits::Data8);
    serial->setStopBits(QSerialPort::StopBits::OneStop);
    serial->setFlowControl(QSerialPort::FlowControl::HardwareControl);
}

void Backend::changeFaderCc(uint pin, uint value) {
    switch (pin) {
        case 18:
            m_faderOneValue = value;
            faders[pin] = m_faderOneValue;
            break;
        case 19:
            m_faderTwoValue = value;
            faders[pin] = m_faderTwoValue;
            break;
        case 20:
            m_faderThreeValue = value;
            faders[pin] = m_faderThreeValue;
            break;
        case 21:
            m_faderFourValue = value;
            faders[pin] = m_faderFourValue;
            break;
        default:
            return;
    }
}

void Backend::receive()
{
    while (serial->canReadLine()) {
        QByteArray data = serial->readLine();
        QString values = QString::fromStdString(data.toStdString());
        auto simplifiedString = values.simplified();
        QList splitted_data = simplifiedString.split(",");
        uint cc = splitted_data[0].toUInt();
        uint cc_value = splitted_data[1].toUInt();
        m_message[0] = 176;
        m_message[1] = faders[cc];
        m_message[2] = cc_value;
        qDebug() << m_message;
        midiout->sendMessage(&m_message);
        emit getCCValue(cc, cc_value);
    }
}
