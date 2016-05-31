#include "udpservice.h"

UdpService::UdpService(QString login, QObject *parent)
{
    this->login = login;
    aliveTimer = new QTimer(this);
    connect(aliveTimer, &QTimer::timeout, this, &UdpService::iAmAlive);

    socket = new QUdpSocket(this);
    if(!socket->bind(UDP_PORT, QUdpSocket::ShareAddress)){
        throw 1; // ###
    }
    connect(socket, &QUdpSocket::readyRead, this, &UdpService::read);
}

void UdpService::iAmAlive()
{
    QNetworkInterface * networkInterface = new QNetworkInterface();
    QList<QHostAddress> ownAddressList = networkInterface->allAddresses();

//    AlivePackage pack = AlivePackage(login, roomName, ownAddressList);
//    pack.write(socket);

    QByteArray broadcast;
    QDataStream out(&broadcast, QIODevice::WriteOnly);

    // login.size-login-roomName.size-roomName-hostAddress.count-hostAddress.asInt
    out << login.toUtf8().size();
    out.writeRawData(login.toUtf8().constData(), login.toUtf8().size());
    out << roomName.size();
    out.writeRawData(roomName.toUtf8().constData(),roomName.toUtf8().size());
    out << ownAddressList.size();
    for(auto address : ownAddressList){
        out << address.toIPv4Address();
    }

    socket->writeDatagram(broadcast, broadcast.size(), QHostAddress::Broadcast, UDP_PORT);
}

void UdpService::setRoomAndLogin(QString room, QString login)
{
    this->login = login;
    this->roomName = room;
    this->startAliveTimer();
}

void UdpService::read()
{
    QByteArray datagram;
    QDataStream in(&datagram, QIODevice::ReadOnly);
    qint32 lsize;
    qint32 rsize;
    qint32 hostAdressesCount;
    QList<QHostAddress> addrlist;
    while (socket->hasPendingDatagrams()) {
        addrlist.clear();
        datagram.clear();
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());
        in >> lsize;
        char * login = new char[lsize+1];
        in.readRawData(login, lsize);
        login[lsize] = 0;
        in >> rsize;
        char * roomName = new char[rsize+1];
        in.readRawData(roomName, rsize);
        roomName[rsize] = 0;
        in >> hostAdressesCount;
        for(int i = 0; i < hostAdressesCount; ++i){
            qint32 tempaddr;
            in >> tempaddr;
            addrlist.append(QHostAddress(tempaddr));
        }
        emit newMember(QString(login), QString(roomName), addrlist);
    }

}

void UdpService::run()
{
    this->read();
}

void UdpService::startAliveTimer()
{
    aliveTimer->start(SEND_ALIVE_TIMEOUT_MSEC);
}
