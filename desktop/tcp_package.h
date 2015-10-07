#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QImage>
#include <QTcpSocket>
#include <assert.h>
#include <QDateTime>

#include "constants.h"

#ifndef TCP_PACKAGE_H
#define TCP_PACKAGE_H

enum pckg_t
{
    TCP_IDLE,
    TEXT,
    IMAGE,
    FILENOTIF,
    FILEREQ,
    FILERESP,
    PASS,
    MEMBER,
    INVALID_PASS,
    REMOVE
};

const int PCKG_SZ_FIELD_SZ = sizeof(qint32);
const int PCKG_TYPE_FIELD_SZ = sizeof(pckg_t);
const int BYTE_ARR_SZ_FIELD_SZ = sizeof(qint32);

class TcpPackage : public QObject
{
    Q_OBJECT
protected:
    qint32 address;
    int size;
    int size2;
    char* text;
    char* login;
    char* image;
    char* password;
public:
    virtual void write(QTcpSocket *) = 0;
    virtual void read(QDataStream &) = 0;
    virtual ~TcpPackage();
    TcpPackage();
signals:
    void gotText(QString);
    void gotImage(QByteArray);
    void gotPass(QString, FloatServerPriority);
    void gotData(QByteArray, QString);
    void gotFileNotification(QString, QDateTime);
    void addMember(FloatServerPriority, QHostAddress);
    void deleteMember(QHostAddress);
    void gotFileReq(QString, QDateTime);
    void gotFileResp(QString, QDateTime, QByteArray);
};

class FailPackage : public TcpPackage
{
public:
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class DataPackage : public TcpPackage
{
    QByteArray data;
    pckg_t type;
public:
    DataPackage(QByteArray&, pckg_t);
    DataPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class MemberPackage : public TcpPackage
{
    QHostAddress addr;
    FloatServerPriority prior;
public:
    MemberPackage();
    MemberPackage(QHostAddress, FloatServerPriority);
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class RemoveMemberPackage : public TcpPackage
{
    QHostAddress removeAddr;
public:
    RemoveMemberPackage();
    RemoveMemberPackage(QHostAddress);
    void write(QTcpSocket *);
    void read(QDataStream &);
};

class ImagePackage : public TcpPackage
{
public:
    ImagePackage();
    void write(QTcpSocket *);
    void read(QDataStream &);
};

class PassPackage : public TcpPackage
{
    FloatServerPriority priority;
    QString password;
public:
    PassPackage(QString, FloatServerPriority);
    PassPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class TcpPackageFactory
{
public:
    TcpPackage * getPackage(pckg_t type);
};


class FileNotificationPackage : public TcpPackage
{
    QByteArray data;
    //QHostAddress sourceAddress;
    QDateTime timeStamp;

public:
    FileNotificationPackage();
    FileNotificationPackage(QByteArray, QDateTime);

    void read(QDataStream &);
    void write(QTcpSocket *);
};

class FileReqPackage : public TcpPackage
{
    QString fileName;
    QDateTime timeStamp;
public:
    FileReqPackage(QString, QDateTime);
    FileReqPackage();
    ~FileReqPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

class FileRespPackage : public TcpPackage
{
    QString fileName;
    QDateTime timeStamp;
    QByteArray data;
public:
    FileRespPackage(QString, QDateTime, QByteArray&);
    FileRespPackage();
    void read(QDataStream &);
    void write(QTcpSocket *);
};

#endif // TCP_PACKAGE_H
