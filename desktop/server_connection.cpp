#include "server_connection.h"

ServerConnection::ServerConnection(QHostAddress host) : Connection(NULL)
{
    this->host = host;
}

void ServerConnection::sendPassLoginPriority(QString password,
                                             QString login,
                                             FloatServerPriority priority)
{
    PassPackage(password, priority).write(socket);
}

//void ServerConnection::onData()
//{
//    QDataStream in(socket);
//    if(transferFinished)
//    {
//        in >> currenFiletSize;
//        transferFinished = false;
//        file.clear();
//    }
//    downloadMore(file, socket);
//}

void ServerConnection::dispatch(QDataStream &in)
{
    qint32 packt;
    in >> packt;
    pckg_t p = (pckg_t)packt;
    hand = TcpPackageFactory().getPackage((pckg_t)packt);
    connect(hand, SIGNAL(gotText(QString)), this, SIGNAL(gotText(QString)));
    connect(hand, SIGNAL(gotImage(QByteArray)),
            this, SIGNAL(gotImage(QByteArray)));
    connect(hand, SIGNAL(gotData(QByteArray,QString)),
            this, SIGNAL(gotData(QByteArray,QString)));
    connect(hand, SIGNAL(gotFileNotification(QString,QDateTime)),
            this, SIGNAL(gotFileNotification(QString,QDateTime)));
    connect(hand, SIGNAL(gotFileReq(QString,QDateTime)),
            this, SIGNAL(gotFileRequest(QString,QDateTime)));
    connect(hand, SIGNAL(gotFileResp(QString,QDateTime,QByteArray)),
            this, SIGNAL(gotFileResponse(QString,QDateTime,QByteArray)));
    connect(hand, SIGNAL(addMember(FloatServerPriority,QHostAddress)),
            this, SIGNAL(addMember(FloatServerPriority,QHostAddress)));
    connect(hand, SIGNAL(deleteMember(QHostAddress)),
            this, SIGNAL(deleteMember(QHostAddress)));
    hand->read(in);
}

void ServerConnection::sendData(QByteArray data, pckg_t type)
{
    DataPackage(data, type).write(socket);
}

void ServerConnection::sendFileNotification(QByteArray & data, QDateTime stamp)
{
    FileNotificationPackage(data, stamp).write(socket);
}

void ServerConnection::sendFileRequest(QString name, QDateTime stamp)
{
    FileReqPackage(name, stamp).write(socket);
}

void ServerConnection::respondFile(QString name, QDateTime stamp, QByteArray & data)
{
    FileRespPackage(name, stamp, data).write(socket);
}

void ServerConnection::run()
{
    socket = new QTcpSocket(this);
    //transferFinished = true;
    connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(serverFell()));

    socket->connectToHost(host, PORT_NUMBER);
    if(!socket->waitForConnected(3000))
        qDebug() << socket->error();
}
