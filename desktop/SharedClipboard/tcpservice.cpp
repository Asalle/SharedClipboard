#include "tcpservice.h"

TcpService::TcpService(QObject *parent) : QObject(parent)
{
}

void TcpService::createServer()
{
    ownServer = new QTcpServer(this);
    if(!ownServer->listen(QHostAddress::Any, TCP_PORT)){
        throw 2; //###
    }
    connect(ownServer, SIGNAL(newConnection()),
            this, SLOT(registerConnection()));
}

void TcpService::connectSocket(QTcpSocket * socket, QHostAddress dest)
{
    socket->connectToHost(dest, TCP_PORT);
    socket->waitForConnected(WAIT_FOR_CONN_TIME_MSEC);

    if(socket->state() == QAbstractSocket::UnconnectedState)
        throw 3; //###
}

void TcpService::send(QByteArray & data)
{
    for(auto socket : roomSockets){
        if(socket->write(data) < data.size()){
            throw 22;
        }
    }

}

void TcpService::setRoomMembers(QList<RoomMember> value)
{
    for(auto member: value){
        if(!roomMembers.contains(member)){
            addRoomMembers(member.addresses);
            roomMembers.append(member);
        }
    }
}

// Rooms Sockets
void TcpService::addRoomMembers(QList<QHostAddress> addressList)
{
    QTcpSocket * socket = new QTcpSocket(this);
    for(auto address : addressList){
        this->connectSocket(socket, address);
    }
    this->roomSockets.append(socket);
    connect(socket, &QTcpSocket::disconnected, [=](){
       if(!this->roomSockets.removeOne(socket)){
           throw 23; //###
       }
    });
}

// Connected Sockets
void TcpService::registerConnection()
{
    QTcpSocket * nextConnection = ownServer->nextPendingConnection();
    if(!connectedSockets.contains(nextConnection)){
        connectedSockets.append(nextConnection);
        connect(nextConnection, &QTcpSocket::readyRead,
                this, &TcpService::read);
        connect(nextConnection, &QTcpSocket::disconnected,
                [=](){ if(!connectedSockets.removeOne(nextConnection)) throw 4; }); // ###
    }
}

void TcpService::read()
{
    QByteArray data;
    QTcpSocket * sender = dynamic_cast<QTcpSocket*>(QObject::sender());
    data = sender->readAll();
    TcpPackage packagetype;

}
