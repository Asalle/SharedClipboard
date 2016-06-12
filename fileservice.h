#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QObject>
#include <QDateTime>
#include <QDataStream>
#include <QFileInfo>
#include <QFile>

#include "common.h"
#include "encryptionservice.h"

class FileService : public QObject
{
    Q_OBJECT
public:
    explicit FileService(QObject *parent = 0);
    ~FileService();

    QByteArray fileDataToByteArray(QString);
    SharedFile byteArrayToFileData(QByteArray&);
    QByteArray getFileChunk(qint32 start, size_t size, QString fullpath);
signals:
    void fileListUpdated(QList<SharedFile>);
    void reqFile(SharedFile);
    void fileResp(QFile*, QList<int>);
public slots:
    //void updateFile(QByteArray chunk); // and what else?
    void updateFileList(QByteArray); /*!< when a new file becomes available, we need to update this list to stay tuned*/
    void fileChosen(SharedFile); /*!< take the shadowid and create the sharedfile to ask for*/
    void findFile(QByteArray data);
private:
    QList<SharedFile> availableFiles;

    // true - ok, false - not ok
    bool insertFile(SharedFile);
    void sendFileChunks(QFileInfo, QList<int>);
};

#endif // FILESERVICE_H
