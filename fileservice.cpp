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
        int sourceAdresses;
        in >> sourceAdresses;
        QList<QHostAddress> adresslist;
        for(int i = 0; i < sourceAdresses; ++i){
            qint32 address;
            in >> address;
            adresslist.append(QHostAddress(address));
        }
        SharedFile candidateFile(QString::fromUtf8(filename, namesize),
                                            QDateTime::fromMSecsSinceEpoch(lastChanged), adresslist);
        if (insertFile(candidateFile)){
            newFiles.append(candidateFile);
        }

        //delete filename;
    }
    emit fileListUpdated(newFiles);
}

void FileService::fileChosen(SharedFile candidateFile)
{
    for(SharedFile file : availableFiles)
    {
        if (file == candidateFile){
            emit reqFile(file);
        }
    }
}

void FileService::findFile(QByteArray data)
{
    // decypher the data
    QDataStream in(&data, QIODevice::ReadOnly);
    int namesz;
    in >> namesz;
    char * fileName = new char(namesz);
    in.readRawData(fileName, namesz);
    qint64 lastchange;
    in >> lastchange;
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
    QFileInfo info(QString(SC_snd_dir)+QString(fileName));
    if (info.exists() && info.isReadable()){
        sendFileChunks(info, requesters);
    } else {
        // else form a fail package
       emit fileResp(nullptr, QList<int>());
    }
}

void FileService::updateFile(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    int namesz;
    in >> namesz;
    char * name = new char(namesz);
    in.readRawData(name, namesz);
    int chunksz;
    in >> chunksz;
    char * chunk = new char(chunksz);
    in.readRawData(chunk, chunksz);

    QString fpath = QString::fromUtf8(SC_rcv_dir, 17);
    fpath += QString::fromUtf8(name, namesz);
    QFile file(fpath);

    if(QFileInfo(fpath).exists()){
        file.open(QIODevice::Append);
    } else {
        file.open(QIODevice::WriteOnly);
    }
    file.write(chunk, chunksz);
    file.close();

//    delete name;
//    delete chunk;

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

    QFile *subjFile = new QFile(info.filePath());
    emit fileResp(subjFile, requesters);
}


// SharedFile declaration

QList<QHostAddress> SharedFile::getSources() const
{
    return sources;
}

void SharedFile::setSources(const QList<QHostAddress> &value)
{
    sources = value;
}

SharedFile::SharedFile(QString name, QDateTime lc, QList<QHostAddress> s){
    this->name = name;
    this->lastChange = lc;
    this->sources = s;
}

SharedFile::SharedFile(QString name, QDateTime lc)
{
    this->name = name;
    this->lastChange = lc;
}

bool SharedFile::operator ==(SharedFile const & other){
    return this->name == other.name &&
           this->lastChange == other.lastChange;
}

SharedFile::SharedFile(const SharedFile & other){
    this->name = other.name;
    this->lastChange = other.lastChange;
    this->sources = other.getSources();
}
