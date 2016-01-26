#ifndef CLIPBOARDSERVICE_H
#define CLIPBOARDSERVICE_H

#include <QObject>
#include <QClipboard>
#include <QApplication>
#include <QByteArray>
#include <QMimeData>
#include <QBuffer>
#include <QImage>
#include <QUrl>


#include "tcppackages.h"


//! A class, that interacts with system clipboard
//! Main functions are: put to clipboard and take from clipboard

class ClipboardService : public QObject
{
    Q_OBJECT

    QClipboard * clipboard = NULL; /*!< Pointer to the system clipboard object*/
    bool sharing = true; /*!< switches sharing on and off*/
    bool sent = false; /*! prevents infinite recursive duplication*/
public:
    explicit ClipboardService(QObject *parent = 0);
    ~ClipboardService();

signals:
    void clipboardChanged(TcpPackage type, QByteArray & data);
    void hasUrls(QList<QUrl>);

public slots:
    void updateClipboard(TcpPackage type, QByteArray & data);
    void getClipboardData();
private:
    QByteArray * toByteArray(QImage&);
    QImage * fromByteArray(QByteArray&);
};

#endif // CLIPBOARDSERVICE_H
