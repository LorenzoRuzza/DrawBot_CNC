#ifndef STRUCTPATH_H
#define STRUCTPATH_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Structpath : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString gcode READ gcode WRITE setGcode NOTIFY gcodeChanged)
    Q_PROPERTY(bool paused MEMBER closed_);
public:
        explicit Structpath(QObject *parent = nullptr);
        QString gcode();        

signals:
        void gcodeChanged();

public slots:

        void setGcode(QString);

        void openSerialPort();
        void closeSerialPort();

        void writeData();
        void readData();

        void test_index();

        void save(const QUrl &);

        void handleError(QSerialPort::SerialPortError error);

private:
        QString gcode_;
        int index_string_ = 0;
        bool closed_ = false;

        void showStatusMessage(const QString &message);

        QSerialPort *serial;        
};

#endif // STRUCTPATH_H
