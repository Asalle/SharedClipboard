#include "roomchoose.h"
#include "ui_roomchoose.h"

RoomChoose::RoomChoose(QList<QString>, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomChoose)
{
    ui->setupUi(this);

    listModel = new QStandardItemModel();

    // name, last change
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(MAX_FILE_HISTORY);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels({"File name", "File last changed"});

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Enter the room");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->listView, &QListView::clicked,
            [&](const QModelIndex & index){
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            });
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
    if(ui->listView->currentIndex().isValid())
        emit roomChosen(qvariant_cast<QString>(ui->listView->model()->data(ui->listView->currentIndex())));
}

void RoomChoose::on_bt_addRoom_clicked()
{
    if (addRoomDlg == nullptr){
        addRoomDlg = new AddRoom(this);
        connect(addRoomDlg, &AddRoom::newRoom,
                this, &RoomChoose::newRoom);
        connect(addRoomDlg, &AddRoom::newRoom,
                [&](QString name){
                    ui->bt_addRoom->setEnabled(false);
                });
        connect(addRoomDlg, &AddRoom::newPass, this, &RoomChoose::newPass);
    }
    addRoomDlg->show();
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

void RoomChoose::updateFileTable(QList<SharedFile> files)
{
    for (SharedFile file : files){
        ui->tableWidget->setItem(tableRowCount, 0, new QTableWidgetItem(file.name));
        ui->tableWidget->setItem(tableRowCount, 1, new QTableWidgetItem(file.lastChange.toString()));
        tableRowCount++;
    }

//     delete older rows
    if (ui->tableWidget->rowCount() > MAX_FILE_HISTORY){
        ui->tableWidget->removeRow(0);
        tableRowCount--;
    }
}

void RoomChoose::on_buttonBox_2_accepted()
{
    on_tableWidget_doubleClicked(ui->tableWidget->currentIndex());
}

void RoomChoose::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if(ui->tableWidget->item(index.row(), 1))
    {
        // column 0 -- name, column 3 -- last change
        SharedFile temp(ui->tableWidget->item(index.row(), 0)->text(),
                        QDateTime::fromString(ui->tableWidget->item(index.row(), 1)->text()));
        emit fileChosen(temp);
    }
}

