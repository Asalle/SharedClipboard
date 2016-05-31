#ifndef ROOMCHOOSE_H
#define ROOMCHOOSE_H

#include <QDialog>
#include <QList>
#include <QStandardItemModel>

#include "addroom.h"

namespace Ui {
class RoomChoose;
}

class RoomChoose : public QDialog
{
    Q_OBJECT

public:
    explicit RoomChoose(QList<QString>, QWidget *parent = 0);
    ~RoomChoose();
signals:
    void newRoom(QString room, QString login);
    void newPass(QString pass);
    void roomChosen(QString);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_bt_addRoom_clicked();

public slots:
    void updateRoomList(QString roomName);

private:
    Ui::RoomChoose *ui;
    AddRoom * addRoomDlg = nullptr;
    int count = 0; // ###

    QStandardItemModel *listModel;
};

#endif // ROOMCHOOSE_H
