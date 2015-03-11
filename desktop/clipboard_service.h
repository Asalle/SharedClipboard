#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>
#include <QList>

struct ClipboardData {
    qint32 dataID;
    QByteArray data;
    QString type;
};

class ClipboardService : public QObject
{
    Q_OBJECT
public:
    ClipboardService();
signals:
    void hasDataToText(QString, qint32);
    void hasData(QByteArray, QString);
    void deleteDataFromStorage(qint32);
    void hasText(QString);
    void hasImage(QByteArray);
public slots:
    void onClipboardChanged();
    void onSettingsChoosed(int value, bool isInKB);
    void pushDataToClipboardFromGui(qint32 dataId);
    void pushDataToClipboardFromHosts(QByteArray data, QString type);
    void pushText(QString);
private:
    QClipboard* clipboard;
    QVector<ClipboardData>clipboardData;
    int clipboardOpacity = 10;
    bool isInKB;

    QString minimizeText(QString text);
};

#endif // CLIPBOARDSERVICE_H
