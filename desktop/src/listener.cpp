#include "listener.h"

Listener::Listener(QObject *parent) :
    QObject(parent),
    m_udp_socket(0),
    m_last_id(-1)
{
    connect(this,&Listener::peerFound,this,&Listener::onPeerFound);
    connect(this,&Listener::helloReceived,this,&Listener::onHelloReceived);
}

void Listener::listen()
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces()) {
        if (!iface.isValid() &&
                !iface.flags().testFlag(QNetworkInterface::IsUp) &&
                iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            continue;
        foreach (QNetworkAddressEntry addr_ent, iface.addressEntries()) {
            if(addr_ent.broadcast().isNull())
                continue;
            m_broadcasts<<addr_ent.broadcast();
        }
    }
    m_udp_socket=new QUdpSocket();
    if (!m_udp_socket->bind(UDP_PORT,QUdpSocket::ShareAddress))
        qFatal("Can't bind UDP socket.");
    connect(m_udp_socket,&QUdpSocket::readyRead,this,&Listener::processPendingDatagrams);
    peerLookupUDP();
}

void Listener::peerLookupUDP()
{
    DatagramPacket packet;
    packet.type=PacketType::Lookup;
    packet.id=qrand();
    packet.content=QHostInfo::localHostName();
    foreach (QHostAddress addr, m_broadcasts) {
        send(addr,packet);
    }
}

void Listener::sendHello(QHostAddress peer)
{
    DatagramPacket packet;
    packet.type=PacketType::Hello;
    packet.id=qrand();
    packet.content=QHostInfo::localHostName();
    send(peer,packet);
}

void Listener::sendAreYouHere(QHostAddress peer)
{
    DatagramPacket packet;
    packet.type=PacketType::AreYouHere;
    packet.id=qrand();
    packet.content=QHostInfo::localHostName();
    send(peer,packet);
}

void Listener::sendClipboard(QHostAddress peer, QString text)
{
    DatagramPacket packet;
    packet.type=PacketType::Clipboard;
    packet.content=text;
    packet.id=qrand();
    send(peer,packet);
}

void Listener::processPendingDatagrams()
{
    while (m_udp_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(m_udp_socket->pendingDatagramSize());
        QHostAddress sender_adr;
        m_udp_socket->readDatagram(data.data(), data.size(),&sender_adr);
        DatagramPacket p;
        QDataStream stream(&data,QIODevice::ReadOnly);
        stream>>p;
        if(p.id==m_last_id)
            continue;
        m_last_id=p.id;
        if(p.type==PacketType::Lookup && p.content==QHostInfo::localHostName())
        {
            Logger::instance()<<TimeStamp()<<"Self test - OK!"<<Flush();
            return;
        }
        switch(p.type)
        {
        case PacketType::Lookup:
            emit peerFound(sender_adr,p.content);
            emit resetTimerForPeer(sender_adr);
            break;
        case PacketType::Hello:
            emit helloReceived(sender_adr,p.content);
            emit resetTimerForPeer(sender_adr);
            break;
        case PacketType::Clipboard:
            emit clipboardContentArrived(p.content);
            emit resetTimerForPeer(sender_adr);
            break;
        case PacketType::AreYouHere:
            emit areYouHereReceived(sender_adr,p.content);
            emit resetTimerForPeer(sender_adr);
            break;
        default:
            break;
        }
    }
}

void Listener::onDisable()
{
     disconnect(m_udp_socket,&QUdpSocket::readyRead,this,&Listener::processPendingDatagrams);
     m_udp_socket->close();
}

void Listener::onEnable()
{
    if (!m_udp_socket->bind(UDP_PORT,QUdpSocket::ShareAddress))
        qFatal("Can't bind UDP socket.");
    connect(m_udp_socket,&QUdpSocket::readyRead,this,&Listener::processPendingDatagrams);
    peerLookupUDP();
}

void Listener::onPeerFound(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": lookup"<<Flush();
    sendHello(peer);
}

void Listener::onHelloReceived(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": hello!"<<Flush();
    //sendHello(peer);
}

void Listener::onAreYouHereReceived(QHostAddress peer, QString nick)
{
    Logger::instance()<<TimeStamp()<<nick<<": are you here?"<<Flush();
    sendHello(peer);
}

void Listener::send(const QHostAddress &peer, const DatagramPacket &packet)
{
    QBuffer data_buffer;
    Q_ASSERT(data_buffer.open(QIODevice::WriteOnly));
    QDataStream stream(&data_buffer);
    stream<<packet;
    QUdpSocket socket;
    for(int i=0;i<REPEAT;i++)
        socket.writeDatagram(data_buffer.data(),peer,UDP_PORT);
}

