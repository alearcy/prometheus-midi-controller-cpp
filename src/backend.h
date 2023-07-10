#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "RtMidi.h"

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();
    std::string midiPortName;

signals:
    void getCCValue(uint pin, uint value);
    void getMidiDevices(QList<QString> devices);
    void getIsSerialConnected(bool isConnected);
public slots:
    void importsMidiDevices();
    void toggleSerial();
    void setMidiDevice(unsigned int currentIndex);
    void changeFaderCc(uint pin, uint value);
    void receive();
private:
    QList<QString> m_midiDevices;
    unsigned int m_midiDevice;
    std::vector<unsigned char> m_message = {0, 0, 0};
    // set default cc values
    uint m_faderOneValue = 1;
    uint m_faderTwoValue = 11;
    uint m_faderThreeValue = 2;
    uint m_faderFourValue = 3;
    std::unordered_map<uint, uint> faders = {
            {18, m_faderOneValue},
            {19, m_faderTwoValue},
            {20, m_faderThreeValue},
            {21, m_faderFourValue},
    };
    RtMidiOut *midiout;
    QSerialPort *serial;
    QSerialPortInfo *serialInfo;
    QList<QSerialPortInfo> serialPorts;
    QString m_serialPortName;
    bool serialToggled = false;
    void configureSerialPort();
    char buffer[2];
    QString dataRead;
};

#endif // BACKEND_H
