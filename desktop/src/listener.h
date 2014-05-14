#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <QList>
#include <QDebug>

#include "common.h"
#include "logger.h"

class Listener : public QObject
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = 0);
signals:
    void peerFound(QHostAddress peer, QString nick);
    void helloReceived(QHostAddress peer, QString nick);
    void areYouHereReceived(QHostAddress peer, QString nick);
    void clipboardContentArrived(QString text);
    void resetTimerForPeer(QHostAddress peer);
public slots:
    void sendHello(QHostAddress peer);
    void sendAreYouHere(QHostAddress peer);
    void sendClipboard(QHostAddress peer, QString text);
    void listen();
    void onDisable();
    void onEnable();
private slots:
    void processPendingDatagrams();
    void onPeerFound(QHostAddress peer, QString nick);
    void onHelloReceived(QHostAddress peer, QString nick);
    void onAreYouHereReceived(QHostAddress peer, QString nick);
private:
    void peerLookupUDP();
    void send(const QHostAddress & peer, const DatagramPacket & packet);

    QUdpSocket * m_udp_socket;
    QList<QHostAddress> m_broadcasts;
    qint32 m_last_id;
};

#endif // LISTENER_H