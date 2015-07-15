#ifndef CLIPBOARDTRAYICON_H
#define CLIPBOARDTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QApplication>

#include "roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"
#include "mainwindow.h"

class ClipboardTrayIcon : public QMainWindow
{    
    Q_OBJECT

    QSystemTrayIcon * icon = NULL;
    MainWindow * mainwindow = NULL;
    RoomsListDialog * roomDialog = NULL;
    CreateRoomDialog * createRoomDialog = NULL;

    QMenu * trayIconMenu;
    QAction * connectAction;
    QAction * createRoomAction;
    QAction * historyAction;
    QAction * maximizeAction;
    QAction * stopSharingAction;
    QAction * settingsAction;
    QAction * aboutAction;
    QAction * quitAction;
    QAction * deleteServerAction = NULL;

    void createMenu();
    void connectMainWindow(MainWindow*);
public:
    ClipboardTrayIcon();
    ~ClipboardTrayIcon();

    void show();
signals:
    void stopSharing();
    void settings();
    void about();
    void quit();

    void serverRoomCreated(QString, QString);
    void roomListOpened(RoomsListDialog *);
    void deleteServerSignal();
public slots:
    void connectRoom();
    void createRoom();
    void showMaximized();

    void becomeServer(QString);
    void stopBeignServer();
};

#endif // CLIPBOARDTRAYICON_H
