#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>
#include <QApplication>
#include <QClipboard>
#include <QDebug>

#include "logger.h"

class ClipboardManager : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardManager(QObject *parent = 0);

public slots:
    void clipboardContentArrived(QString text);
    void onDisable();
    void onEnable();
signals:
    void sendClipboard(QString text);
private slots:
    void onClipboardChanged(QClipboard::Mode mode);
private:
    QClipboard * m_clipboard;
};

#endif // CLIPBOARDMANAGER_H