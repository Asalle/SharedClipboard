#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QFile>
#include <QNetworkInterface>

#include "tcppackages.h"
#include "common.h"
#include "encryptionservice.h"

class TcpService : public QObject
{
    Q_OBJECT

    QTcpServer * ownServer = NULL;
    QList<RoomMember> roomMembers;
    QList<QTcpSocket*> roomSockets; // came over udp, to send to
    QList<QTcpSocket*> connectedSockets; // connected to my server, to receive from
    QList<QTcpSocket*> fileSockets; // to send files
    QSharedPointer<EncryptionService> encService;

    void gotFiles();

public:
    explicit TcpService(QObject *parent = 0);

    void createServer();

    void connectSocket(QTcpSocket*, QHostAddress dest);
    void addRoomMembers(QList<QHostAddress>);

    void send(TcpPackage type, QByteArray&);
    void setEncService(QSharedPointer<EncryptionService> enc);
signals:
    void gotData(TcpPackage, QByteArray &);
    void gotFileNotif(QByteArray);
    void gotFileReq(QByteArray);
    void pwdAC(QString room);
    void pwdErr(QString room);

public slots:
    void setRoomMembers(QList<RoomMember>);
    void checkPass(QString);
    void reqFile(SharedFile);
    void sendFileChunks(QFile*, QList<int>);

private slots:
    void read();
    void registerConnection();
};

#endif // TCPSERVICE_H
