#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QNetworkInterface>
#include <QByteArray>
#include <QTimer>
#include <QList>


#include "udpservice.h"
#include "tcpservice.h"
#include "shared_clipboard_exception.h" //###

//! Provides member management and is responsible
//! for network services for members

class RoomService : public QObject
{
    Q_OBJECT

    QSharedPointer<UdpService> udpService; /*!< udpservice of me*/
    QSharedPointer<TcpService> tcpService; /*!< tcpservice of me*/

    QList<RoomMember> roomMembers; /*!< list of my room members*/
    QList<QString> roomsNames; /*!< names of rooms avaliable*/

    QString ownRoomName; /*!< name of my room*/
    QTimer * checkAlivesTimer; /*!< timer for checking alives*/
public:
    explicit RoomService(QObject *parent = 0);
    QList<QString> getRooms(); /*!< get list of available rooms*/
    QList<RoomMember> getRoomMembers() const; /*!< get list of my room members*/

signals:
    void refreshMembers(QList<RoomMember>);
    void PwdAC(QString login, QString pass);
    void PwdFailed();

public slots:
    /*!< add new member to members list*/
    void addMember(QString login, QString room, QList<QHostAddress> ip);
    void checkAlives(); /*!< checks if all my room members are alive*/
    void setRoom(QString &); /*!< sets my room from the room list*/
};

#endif // CONTROL_H
