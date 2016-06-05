#ifndef PORTS_H
#define PORTS_H

#define UDP_PORT 8534
#define TCP_PORT 8023

#define WAIT_FOR_CONN_TIME_MSEC 5000

// for room service
#define CHECK_ALIVE_TIMEOUT_MSEC 5000

// min n packages not to remove member from alives
#define MIN_RESP_COUNT 2

// for udpService
#define SEND_ALIVE_TIMEOUT_MSEC 500

// sha3 digest size is somehow 20 :(
#define SHA3_DIGEST_SIZE 20

// icon path
#define ICON_PATH "GUI/images/sharedclipboard_icon.svg"

// maximum file history capasity
#define MAX_FILE_HISTORY 20


#include <QDateTime>
#include <QTcpSocket>
class SharedFile
{
    int shadowId;
public:
    QString name;
    QDateTime lastChange;
    QTcpSocket * source;

    int getShadowId() const;
    void setShadowId(int);

    explicit SharedFile(QString name, QDateTime lc);
    SharedFile(SharedFile const & other);
    bool operator ==(SharedFile const & other);
};

#include <QHostAddress>
struct RoomMember
{
    QString login;
    QList<QHostAddress> addresses;
    bool alive;
    int packs_count;

    RoomMember(QString l, QList<QHostAddress> addr, bool a, int pc)
    {

        login = l;
        addresses = addr;
        alive = a;
        packs_count = pc;
    }

    bool operator==(RoomMember const & other)
    {
        return this->login == other.login && this->addresses == other.addresses;
    }

    RoomMember(RoomMember const & other)
    {
        this->addresses = other.addresses;
        this->alive = other.alive;
        this->login = other.login;
        this->packs_count = other.packs_count;
    }
};

#endif // PORTS_H

