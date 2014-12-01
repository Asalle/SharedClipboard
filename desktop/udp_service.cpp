#include "udp_service.h"

/*
 * Serious bug - in function initListener() cycle can't define the ip of local machine, it locates only
 * adress of the router
 * So, in function listener() this adress passes condition and in case of GET_ROOM signal generates with 
 * the wrong adress - adress of the local mashine, not the adress of sender
 * Function sendRoom() sends packet only to first adress in queue, which is localhost adress, that's why
 * client do not display room in the list
 * Temporarily fix - pop one element in sendRoom() before sending
*/

UDPService::UDPService() : QObject(0),
    udp_socket(0)
{
}

bool UDPService::initListener(){

    QTime now = QTime::currentTime();
    qsrand(now.msec());
    
    // Finding the localhosts adresses
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            localhost_ip.push_back(address);
    
    // Filling the distribution list by valid adresses
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())

        if (iface.isValid() &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
            iface.flags().testFlag(QNetworkInterface::IsRunning)
            )

            foreach (QNetworkAddressEntry address_ent, iface.addressEntries())
                if (!address_ent.broadcast().isNull() && address_ent.ip().toIPv4Address())
                    broadcasts << address_ent.broadcast();

    udp_socket = new QUdpSocket();

    if (!udp_socket->bind(UDP_PORT, QUdpSocket::ShareAddress))
        return 1;

    connect(udp_socket, &QUdpSocket::readyRead, this, &UDPService::listener);

    return 0;
}

void UDPService::getRooms(){
    DatagramPacket packet;

    packet.type = GET_ROOM;
    packet.id = qrand();

    sendBroadcastPackadge(packet);
}

void UDPService::notifyAboutRoom(QString name){
    DatagramPacket packet;

    packet.type = ROOM;
    packet.id = qrand();
    packet.name = name;

    sendBroadcastPackadge(packet);
}

void UDPService::notifyAboutRoomDeleting(){
    DatagramPacket packet;

    packet.type = DELETE_ROOM;
    packet.id = qrand();

    sendBroadcastPackadge(packet);
}

void UDPService::sendBroadcastPackadge(const DatagramPacket &packet){

    foreach (QHostAddress addr, broadcasts)
        sendPackage(addr, packet);
}

void UDPService::sendPackage(const QHostAddress &peer, const DatagramPacket &packet){
    QBuffer data_buffer;
    data_buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&data_buffer);

    stream << packet.type;
    stream << packet.id;

    if (packet.type == ROOM){
        QByteArray toBytes = packet.name.toUtf8();
        stream << toBytes.data();
    }

    QUdpSocket socket;

    for (int i = 0; i < REPEAT; ++i)
        socket.writeDatagram(data_buffer.data(), peer, UDP_PORT);
}

void UDPService::sendRoom(QString name, QHostAddress addr){
    DatagramPacket packet;

    packet.type = ROOM;
    packet.id = qrand();
    packet.name = name;
    
    sendPackage(addr, packet);
}

void UDPService::clearReceivedId(){
    received_id.clear();
}

void UDPService::listener(){

    while (udp_socket->hasPendingDatagrams()){

        QByteArray data;
        data.resize(udp_socket->pendingDatagramSize());
        QHostAddress sender_adr;
        udp_socket->readDatagram(data.data(), data.size(), &sender_adr);

        QDataStream stream(&data, QIODevice::ReadOnly);
        DatagramPacket packet;

        packet = unpackPackage(stream);

        if (received_id.contains(packet.id))
            continue;

        bool local = 0;

        foreach (QHostAddress localhost, localhost_ip)
            if (sender_adr == localhost)
                local = 1;

        if (local) continue;

        received_id.push_back(packet.id);

        QTimer::singleShot(300000, this, SLOT(clearReceivedId()));

        switch (packet.type){

            case ROOM:
                emit roomReceived(packet.name, sender_adr);
            break;

            case GET_ROOM:
                emit roomRequested(sender_adr);
            break;

            case DELETE_ROOM:
                emit roomDeleted(sender_adr);
            break;
        }
    }
}
