#include "fileservice.h"

FileService::FileService(QObject *parent) : QObject(parent)
{

}

FileService::~FileService()
{

}

void FileService::updateFileList(QByteArray inba)
{
    QDataStream in(&inba, QIODevice::ReadOnly);
    QList<SharedFile> newFiles;

    int fileCount;
    in >> fileCount;
    for (int i = 0; i < fileCount; ++i){
        int namesize;
        in >> namesize;
        char *filename = new char(namesize);
        in.readRawData(filename, namesize);
        qint64 lastChanged;
        in >> lastChanged;
        SharedFile candidateFile(QString::fromUtf8(filename, namesize),
                                            QDateTime::fromMSecsSinceEpoch(lastChanged));
        if (insertFile(candidateFile)){
            newFiles.append(candidateFile);
        }

        //delete filename;
    }
    emit fileListUpdated(newFiles);
}

void FileService::fileChosen(int shadowId)
{

}

void FileService::findFile(QByteArray data)
{
    // decypher the data
    QDataStream in(&data, QIODevice::ReadOnly);
    int namesz;
    in >> namesz;
    char * fileName = new char(namesz);
    in.readRawData(fileName, namesz);
    int addresslistsz;
    in >> addresslistsz;
    QList<qint32> requesters;
    for (int i = 0; i < addresslistsz; ++i){
        qint32 temp;
        in >> temp;
        requesters.append(temp);
    }

    // check file existance
    // if file is here -- send the response
    QFileInfo info(fileName);
    if (info.exists() && info.isReadable()){
        sendFileChunks(info, requesters);
    } else {
        // else form a fail package
       emit fileResp(nullptr, QList<int>());
    }
}

bool FileService::insertFile(SharedFile file)
{
    // filter the second versions of file
    // return false;
    // append if unique
    availableFiles.append(file);

    // delete old files
    if (availableFiles.size() > MAX_FILE_HISTORY){
        availableFiles.removeFirst();
    }
    return true;
}

void FileService::sendFileChunks(QFileInfo info, QList<int> requesters)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << info.path().size();
    out.writeRawData(info.path().toUtf8().constData(), info.path().size());
    out << info.lastModified().toMSecsSinceEpoch();

    QFile *subjFile = new QFile(info.path());
    emit fileResp(subjFile, requesters);
}


// SharedFile declaration

SharedFile::SharedFile(QString name, QDateTime lc){
    this->name = name;
    this->lastChange = lc;
    this->shadowId = EncryptionService::randomInt();
}

bool SharedFile::operator ==(SharedFile const & other){
    return this->name == other.name &&
           this->lastChange == other.lastChange;
}

SharedFile::SharedFile(const SharedFile & other){
    this->name = other.name;
    this->lastChange = other.lastChange;
    this->shadowId = other.getShadowId();
}

int SharedFile::getShadowId() const{
    return this->shadowId;
}

void SharedFile::setShadowId(int id){
    this->shadowId = id;
}

