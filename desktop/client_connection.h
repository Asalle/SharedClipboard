#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include "connection.h"
#include "constants.h"
#include "tcp_package.h"
#include <assert.h>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QImage>
#include <QBuffer>
#include <QDataStream>
class ClientConnection : public Connection
{
    Q_OBJECT
    qint32 socketDescriptor;
signals:
    void verifyPass(QString pass, FloatServerPriority, ClientConnection * const);
    void onText(QString, ClientConnection * const);
    void onImage(QByteArray, ClientConnection * const);
    void onFileNotification(QString, QDateTime, ClientConnection * const);
    void onFileRequest(QString, QDateTime, ClientConnection * const);
    void onFileRespond(QString, QDateTime, QByteArray, ClientConnection * const);
    void deleteMember(QHostAddress);
public:
    ClientConnection(QTcpSocket * socket);
    ClientConnection(qintptr socket);
    void sendFail();
    void sendMember(FloatServerPriority, QHostAddress addr);
    void removeMember(QHostAddress addr);
    QHostAddress makeHostAdress(char*);
    void sendText(QString);
    void sendImage(QByteArray);
    void sendData(QByteArray arr, pckg_t type);
    void sendFileNotification(QString, QDateTime);
    void getFile(QString);

    void requestFile(QString, QDateTime);
    void respondFile(QString, QDateTime, QByteArray);

    void run() Q_DECL_OVERRIDE;
public slots:
    //void onData();
    void emitText(QString);
    void emitImage(QByteArray);
    void emitFileNotification(QString, QDateTime);
    void emitFileRequest(QString, QDateTime);
    void emitFileResponse(QString, QDateTime, QByteArray);
    void makePass(QString, FloatServerPriority);
    void emitDeleteMember();
private:
    void downloadMore(QByteArray& whole, QTcpSocket * inSocket);
    void dispatch(QDataStream& infile);
};

#endif // CLIENT_CONNECTION_H
