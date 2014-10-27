#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    socket = new QTcpSocket(this); // is socket already initialised in parent class?
    socket->connectToHost(host, (qint16)getpid()); // I don't know what else to do here
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
              //SLOT(throwSocketError(QAbstractSocket::SocketError)));
    //todo error handlers
}

void ServerConnection::sendPassAndLogin(QString password, QString login)
{
    //how to ensure socket is ready to write?
    QByteArray block;
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << login;
    TcpPackage loginPckg = TcpPackage(TcpPackageHeader(LOGIN, block.size()), block);
    socket->write(loginPckg);
    out << password;
    TcpPackage passwordPckg = TcpPackage(TcpPackageHeader(PASS, block.size()), block);
}
