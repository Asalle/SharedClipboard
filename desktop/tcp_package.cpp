#include "tcp_package.h"

void ServerConnectionHandlerText::decode(QDataStream &in)
{
    in >> size;
    text = new char[size];
    in >> text;
    emit gotData(QByteArray::fromRawData(text, size), "text/plain");
}

void ServerConnectionHandlerMember::decode(QDataStream &in)
{
    qint32 priority;
    in >> priority;
    in >> size;
    login = new char[size];
    in >> login;
    in >> address;
    emit addMember(QString::fromUtf8(login), (floating_server_priorities)priority, QHostAddress(address));
}

ServerConnectionHandler *ServerConnectionFactory::getHandler(pckg_t packt)
{
    switch(packt){
        case MEMBER:
            return new ServerConnectionHandlerMember();
       case TEXT:
            return new ServerConnectionHandlerText();
       case REMOVE:
            return new ServerConnectionHandlerRemoveMember();
       case IMAGE:
            return new ServerConnectionHandlerImage();
       default: throw packt;
    }
}

void ServerConnectionHandlerRemoveMember::decode(QDataStream &in)
{
    in >> address;
    emit(deleteMember(QHostAddress(address)));
}

void ServerConnectionHandlerImage::decode(QDataStream &in)
{
    in >> size;
    image = new char[size];
    in.readRawData(image,size);

    emit gotData(QByteArray(image, size), "image/png");
}
