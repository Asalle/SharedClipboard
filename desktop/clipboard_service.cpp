#include "clipboard_service.h"

ClipboardService::ClipboardService() : QObject()
{
    this->clipboard = QApplication::clipboard();
    sharingOn = true;
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
}

void ClipboardService::onClipboardChanged()
{
    if(sharingOn)
    {
        if (locked) {
            locked = false;
            return;
        }
        const QMimeData * mimeData = clipboard->mimeData();
        ClipboardData data;
        QString text;
        data.dataID = (qint32)qrand();

        if (clipboardData.size() == clipboardOpacity) {
            //qDebug() << "vector overflow";
            //emit deleteDataFromStorage(clipboardData.takeLast().dataID);
        }

        if (mimeData->hasUrls()) {
            data.type = "text/uri-list";
            data.data = mimeData->data(data.type);
            text = mimeData->urls().first().toString();
        }
        else if (mimeData->hasImage()) {
            text = "copied image #" + QString::number(qrand());
            data.type = "image/png";
            QImage image = qvariant_cast<QImage>(mimeData->imageData());

            data.data = *imageToQByteArray(image);
            emit hasDataToText(minimizeText(text), data.dataID);

            qDebug() << "size: " << data.data.size();
            const char* rawdata = data.data.constData();
            qDebug() << "data:" << rawdata;
        }
        else if (mimeData->hasText()) {
            data.type = "text/plain";
            data.data = mimeData->text().toUtf8();
            text = mimeData->text();
            emit hasDataToText(minimizeText(text), data.dataID);
        }
        clipboardData.prepend(data);
        emit hasData(data.data, data.type);
    }
}

void ClipboardService::onSettingsChoosed(int value, bool isInKB)
{
    this->clipboardOpacity = value;
    this->isInKB = isInKB;
    this->clipboardData.resize(value);
}

void ClipboardService::pushFromHosts(QByteArray data, QString type)
{
    if(sharingOn)
    {
        locked = true;
        QMimeData * mimeData = new QMimeData();

        mimeData->setData(type, data);
        clipboard->setMimeData(mimeData);

        //emit clipboardRefreshed(type, data);

        qDebug() << "on data from outer host";
    }
}

void ClipboardService::turnSharing()
{
    sharingOn = !sharingOn;
}

void ClipboardService::pushDataToClipboardFromGui(qint32 dataId)
{
    qDebug() << "on item dbclick";
    for(int i = 0; i < clipboardData.size(); i++) {
        if(clipboardData.at(i).dataID == dataId) {
            QMimeData * data = new QMimeData();
            data->setData(clipboardData.at(i).type, clipboardData.at(i).data);
            clipboard->setMimeData(data);
            return;
        }
    }
}

QString ClipboardService::minimizeText(QString text)
{
    const int MAX_STR_LEN = 50;

    int index = text.indexOf('\n');

    if( index >= 0 && index < MAX_STR_LEN)
    {
        if(index < 3 ) { index = text.indexOf('\n',3); }
        text = text.mid(0, index);
        text.append("...");
    }

    else if(text.size() > MAX_STR_LEN)
    {
        text = text.mid(0, MAX_STR_LEN);
        text.append("...");
    }
    return text;
}

QByteArray * ClipboardService::imageToQByteArray(QImage & image)
{
    QByteArray * ba = new QByteArray();
    QBuffer buffer(ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return ba;
}

//QImage ClipboardService::byteArrayToImage(QByteArray & ba)
//{
//    return QImage::fromData(ba.constData(), "PNG");
//}
