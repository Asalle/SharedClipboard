#include "roomchoose.h"
#include "ui_roomchoose.h"

RoomChoose::RoomChoose(QList<QString>, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomChoose)
{
    ui->setupUi(this);
    listModel = new QStandardItemModel();
}

RoomChoose::~RoomChoose()
{
    delete ui;
}

void RoomChoose::on_buttonBox_rejected()
{
    this->close();
}

void RoomChoose::on_buttonBox_accepted()
{
    emit roomChosen(qvariant_cast<QString>(ui->listView->model()->data(ui->listView->currentIndex())));
}

void RoomChoose::on_bt_addRoom_clicked()
{
    if (addRoomDlg == nullptr){
        addRoomDlg = new AddRoom(this);
        connect(addRoomDlg, &AddRoom::newRoom,
                this, &RoomChoose::newRoom);
        connect(addRoomDlg, &AddRoom::newPass, this, &RoomChoose::newPass);
        addRoomDlg->show();
    }
}

void RoomChoose::updateRoomList(QString roomName)
{
    //filter rooms
    if (listModel->findItems(roomName).size()){
        return;
    }

    listModel->appendRow(new QStandardItem(roomName));
    ui->listView->setModel(listModel);
}
