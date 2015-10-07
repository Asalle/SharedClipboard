#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QHostAddress>
#include <QDateTime>
#include <memory>

#define PORT_NUMBER 1080
#define REPEAT 10
#define UDP_PORT 12345
#define MAX_FILE_SIZE 500 * 1024

enum FloatServerPriority
{
    PC,
    MOBILE,
    FLOAT_IDLE
};

struct SharedFile{
    QString name;
    //QHostAddress sourceAddress;
    QDateTime timeStamp;
    //SharedFile(QString, QHostAddress, QDateTime);
    SharedFile(QString, QDateTime);
    SharedFile();
};


#endif // CONSTANTS_H
