#include "tcpservice.h"


TcpService::TcpService(QObject *parent) : QObject(parent)
{
}

void TcpService::createServer()
{
    ownServer = new QTcpServer(this);
    if(!ownServer->listen(QHostAddress::Any, TCP_PORT)){
        throw "server is not listening"; //###
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

void TcpService::send(TcpPackage type, QByteArray & data_raw)
{
    QByteArray sendarray;
    QDataStream out(&sendarray, QIODevice::WriteOnly);
    try {
        data_raw = encService->encrypt(data_raw);
    } catch (...){
        //if the member is not in our room -- reject it
        // if the member is in our room and we don't understand -- except
    }

    out << (qint32)type << data_raw.size();
    out.writeRawData(data_raw.constData(), data_raw.size());
    for(auto socket : roomSockets){
        if(socket->write(sendarray) < sendarray.size()){
            throw 22; // ###
        }
    }

}

void TcpService::setEncService(QSharedPointer<EncryptionService> enc)
{
    this->encService = enc;
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

void TcpService::checkPass(QString)
{

}

void TcpService::reqFile(SharedFile file)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << file.name.size();
    out.writeRawData(file.name.toUtf8().constData(), file.name.size());
    out << file.lastChange.toMSecsSinceEpoch();
    QNetworkInterface interface;
    out << interface.allAddresses().size();
    for (int i = 0; i < interface.allAddresses().size(); ++i){
        out << interface.allAddresses()[i].toIPv4Address();
    }

    data = encService->encrypt(data);

    QByteArray sendarray;
    QDataStream sendstream(&sendarray, QIODevice::WriteOnly);
    sendstream << qint32(TcpPackage::FILE_REQ);
    sendstream << data.size();
    sendstream.writeRawData(data, data.size());

    for (QHostAddress address : file.sources){
        QTcpSocket * filereqsocket = new QTcpSocket();
        connectSocket(filereqsocket, address);
        if (filereqsocket->write(sendarray) == sendarray.size()){
//            filereqsocket->close();
//            delete filereqsocket;
            break;
        }
        filereqsocket->close();
        delete filereqsocket;
    }
}

void TcpService::sendFileChunks(QFile *subjFile, QList<int> requesters)
{
    if(subjFile == nullptr)
        return;

    if (subjFile->exists()){
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QFileInfo(*subjFile).fileName().size();
        out.writeRawData(QFileInfo(*subjFile).fileName().toUtf8().constData(),
                         QFileInfo(*subjFile).fileName().size());

        subjFile->open(QIODevice::ReadOnly);
        QByteArray file = subjFile->readAll();
        out << file.size();
        qDebug() << file.size();
        out.writeRawData(file.constData(), file.size());

        data = encService->encrypt(data);

        QByteArray sendarray;
        QDataStream sendstream(&sendarray, QIODevice::WriteOnly);
        sendstream << qint32(TcpPackage::FILE_RESP);
        sendstream << data.size();
        sendstream.writeRawData(data, data.size());

        for (int address : requesters){
            QTcpSocket * filereqsocket = new QTcpSocket();
            connectSocket(filereqsocket, QHostAddress(address));
            if (filereqsocket->write(sendarray) == sendarray.size()){
    //            filereqsocket->close();
    //            delete filereqsocket;
                break;
            }
        }
    }
    delete subjFile;
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

    QDataStream in(&data, QIODevice::ReadOnly);
    qint32 pack_t;
    in >> pack_t;
    TcpPackage packagetype = (TcpPackage)pack_t;

    int sz;
    in >> sz;
    QByteArray temp = QByteArray(sz, 0);
    in.readRawData(temp.data(), sz);
    try {
        encService->decrypt(temp);
    } catch(...){
        //if the member is not in our room -- reject it
        // if the member is in our room and we don't understand -- except
    }

    if (packagetype == TcpPackage::TXT || packagetype == TcpPackage::PNG)
        emit gotData(packagetype, temp);
    else if (packagetype == TcpPackage::FILE_NOTIF){
        emit gotFileNotif(temp);
    } else if(packagetype == TcpPackage::FILE_REQ){
        emit gotFileReq(temp);
    } else if(packagetype == TcpPackage::FILE_RESP){
        emit gotFileResp(temp);
    } else {
        throw "Unknow package type";
    }
}
