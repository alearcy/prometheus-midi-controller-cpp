#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "RtMidi.h"
#include <memory>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

signals:
    void getCCValue(uint pin, uint value);
    void getMidiDevices(QList<QString> devices);
    void getIsSerialConnected(bool isConnected);
    QString getCCName(uint pin, QString ccName);
public slots:
    void importsMidiDevices();
    void toggleSerial();
    void setMidiDevice(unsigned int currentIndex);
    void changeFaderCc(uint pin, uint value);
    void changeFaderName(uint pin, const QString& ccName);
    void getAvailableSerialPorts();
    void receive();
private:
    QList<QString> m_midiDevices;
    unsigned int m_midiDevice = 0;
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

    QString m_faderTwoName = "Expression";
    QString m_faderThreeName = "Custom";
    QString m_faderFourName = "Custom";
    std::unique_ptr<RtMidiOut> midiOut;
    QSerialPort *serial;
    QSerialPortInfo *serialInfo;
    QList<QSerialPortInfo> serialPorts;
    QString m_serialPortName;
    bool serialToggled = false;
    std::string midiPortName;
    QString m_faderOneName = "Modulation";
    void configureSerialPort();
    void sendMidiMessage(uint cc, uint ccValue);
    QString dataRead;
};

#endif // BACKEND_H
