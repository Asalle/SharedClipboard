#include "clipboardservice.h"

ClipboardService::ClipboardService(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged,this, &ClipboardService::getClipboardData);
}

ClipboardService::~ClipboardService(){}

void ClipboardService::updateClipboard(TcpPackage type, QByteArray &data)
{
    if(sent == true)
    {
        sent = false;
        return;
    }

    if(type == TcpPackage::TXT){
        clipboard->setText("Modified: "+QString(data));
    } else if(type == TcpPackage::PNG){
        clipboard->setImage(*fromByteArray(data));
    } else {
        throw "Unknown MIME-type";
    }
    sent = true;
}

void ClipboardService::getClipboardData()
{
    QMimeData const * data = clipboard->mimeData();
    TcpPackage packageType;
    QByteArray temp;

    if(data->hasUrls()){
        packageType = TcpPackage::FILE_NOTIF;
        QList<QUrl> tempUrlList = data->urls();
        QByteArray temp;
        QDataStream out(&temp, QIODevice::WriteOnly);
        out << tempUrlList.size();
        for (QUrl item : tempUrlList){
            out << QFileInfo(item.path()).baseName().size();
            out.writeRawData(QFileInfo(item.path()).baseName().toUtf8().constData(), item.path().size());
            out << QFileInfo(item.path()).lastModified().toMSecsSinceEpoch();

            QNetworkInterface info;
            out << info.allAddresses().size();
            for (QHostAddress adress : info.allAddresses()){
                out << adress.toIPv4Address();
            }

            qDebug() << item.path();
            qDebug() << QFileInfo(item.path()).lastModified().toString();
        }
        emit clipboardChanged(packageType, temp);

    } else if(data->hasText()){
        packageType = TcpPackage::TXT;
        QString text = data->text();
        temp = QByteArray(text.toUtf8().constData(), text.size());
        emit clipboardChanged(packageType, temp);

    } else if(data->hasImage()){
        packageType = TcpPackage::PNG;
        QVariant tempImageVariant = data->imageData();
        QImage image = tempImageVariant.value<QImage>();
        temp = *(toByteArray(image));
        emit clipboardChanged(packageType, temp);

    } else {
        throw "Unknown MIME format";
    }


}

QByteArray *ClipboardService::toByteArray(QImage & image)
{
    QByteArray * returnArray = new QByteArray();
    QBuffer buffer(returnArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return returnArray;
}

QImage *ClipboardService::fromByteArray(QByteArray & data)
{
    QImage * returnImage = new QImage();
    returnImage->fromData(data);
    return returnImage;
}
