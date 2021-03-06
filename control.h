#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QNetworkInterface>
#include <QByteArray>
#include <QTimer>


#include "udpservice.h"
#include "tcpservice.h"
#include "shared_clipboard_exception.h" //###

class Control : public QObject
{
    Q_OBJECT

    QSharedPointer<UdpService> udpService;
    QSharedPointer<TcpService> tcpService;

    QList<RoomMember> roomMembers;

    QString login;
    QTimer * checkAlivesTimer;
public:
    explicit Control(QString & login, QObject *parent = 0);

signals:

public slots:
    void sendData(TcpPackage, QByteArray&);
    void receiveData(TcpPackage, QByteArray&);

    void addMember(QString,QHostAddress);
    void checkAlives();
};

#endif // CONTROL_H
