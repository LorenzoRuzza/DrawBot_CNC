#include "structpath.h"
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QtTest/QTest>

Structpath::Structpath(QObject *parent)
    : QObject(parent)
    , gcode_("")
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(serial, SIGNAL(errorOccurred()), this, SLOT(handleError()));
    openSerialPort();
}

QString Structpath::gcode()
{
    return gcode_;
}

void Structpath::setGcode(QString newCode)
{
    if (gcode_ != newCode) {
        gcode_ = newCode;
        index_string_ = 0;
        test_index();
        emit gcodeChanged();
    }
}

void Structpath::save(const QUrl &name)
{
    qDebug() << name;
    QFile file(name.path());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    file.write(gcode_.toLatin1());
    file.close();
}

void Structpath::openSerialPort()
{
    serial->setPortName("/dev/ttyACM0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {

        showStatusMessage("Connectedd");

    }
    else {

        showStatusMessage(tr("Open error"));
    }
}

void Structpath::closeSerialPort()
{
    if (serial->isOpen()) {
        serial->close();
    }

    showStatusMessage(tr("Disconnected"));
}

void Structpath::writeData()
{
    QString str = "";

    if (closed_ == false) {
        if (index_string_ <= gcode_.length() - 1) {
            while (gcode_[index_string_] != ';') {
                str += gcode_[index_string_];
                index_string_++;
            }
        }

        str += ';';
        index_string_++;

        if (index_string_ <= gcode_.length()) {
            serial->write(str.toLatin1());
            qDebug() << str;

            readData();
        }
    }
}

void Structpath::readData()
{
   QByteArray data;
   if (serial->bytesAvailable() == 2) {
       data = serial->readAll();
   }

   qDebug() << data;

   if (data == "ok") {
       writeData();
   }
}

void Structpath::test_index()
{
    if (qFuzzyCompare((double)index_string_, 0) == false) {
        closeSerialPort();
    }
}

void Structpath::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort();
    }
}


void Structpath::showStatusMessage(const QString &message)
{
    qDebug() << message;
}
