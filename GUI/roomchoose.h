#ifndef ROOMCHOOSE_H
#define ROOMCHOOSE_H

#include <QDialog>
#include <QList>

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
    void room(QString);
    void roomChosen(QString);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_bt_addRoom_clicked();

private:
    Ui::RoomChoose *ui;
    AddRoom * addRoomDlg = nullptr;
};

#endif // ROOMCHOOSE_H
