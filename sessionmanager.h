#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "roomservice.h"
#include "clipboardservice.h"
#include "fileservice.h"
#include "GUI/loginpassdialog.h"
#include "GUI/roomchoose.h"

class SessionManager
{
    SessionManager();

    LoginPassDialog * dlg; /*!< login-pass-prompt-dialog pointer*/
public:
    /*!< name of session file*/
    static QString sharedClipboardIdFileName;
    /*!< path to session file
     * together withh session file name composes into
     * full path to session file*/
    static QString idFilePath;

    static bool sessionActive(); /*!< if session is active flag*/
    static bool sessionInterrupted(); /*!< if session was interrupted flag*/
    /*!< start a session -- create session file*/
    void startSession( QSharedPointer<RoomService> & roomService,
                            QSharedPointer<UdpService> & udpService,
                            QSharedPointer<TcpService> & tcpService,
                            QSharedPointer<ClipboardService> & clipboardService,
                            QSharedPointer<FileService> & fileService,
                            QSharedPointer<EncryptionService> & encService);

    static bool idFileExists(); /*!< if file exists flag*/
    static bool idFileOpenedForWriting(); /*!< is file opened for writing flag*/

    static QString getLogin(QFile source); //wtf
    static QString setLogin(QString login);//wtf
    static SessionManager * getInstance(); /*!< singleton getInstance*/
};



#endif // SESSIONMANAGER_H
