#ifndef ROOMCHOOSE_H
#define ROOMCHOOSE_H

#include <QDialog>
#include <QListView>
#include <QList>
#include <QStandardItemModel>

#include "addroom.h"
#include "common.h"

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
    void newRoom(QString room);
    void newPass(QString pass);
    void roomChosen(QString);
    void fileChosen(SharedFile);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_bt_addRoom_clicked();

    void on_buttonBox_2_accepted();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

public slots:
    void updateRoomList(QString roomName);
    void updateFileTable(QList<SharedFile>);

private:
    Ui::RoomChoose *ui;
    AddRoom * addRoomDlg = nullptr;
    int count = 0; // ###

    QStandardItemModel *listModel;
    int tableRowCount = 0;
};

#endif // ROOMCHOOSE_H
